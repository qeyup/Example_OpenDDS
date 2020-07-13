#include <thread>
#include <chrono>
#include <iostream>


//> IDL gen code
//#include "msgC.h"
//#include "msgS.h"
//#include "msgTypeSupportS.h"

//#include "msgTypeSupportC.h"
#include "msgTypeSupportImpl.h"


//> DDS headers
//#include "dds/DdsDcpsPublicationC.h"
//#include "dds/DdsDcpsSubscriptionC.h"
//#include "dds/DdsDcpsDomainC.h"
//#include <dds/DdsDcpsSubscriptionC.h>
//#include "dds/DCPS/Service_Participant.h"
//#include "dds/DCPS/Marked_Default_Qos.h"
//#include "dds/DCPS/PublisherImpl.h"
//#include "ace/streams.h"
//#include "orbsvcs/Time_Utilities.h"


//> Transport and discovery static linking
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#include <dds/DCPS/RTPS/RtpsDiscovery.h>


#define DDS_DOMAIN  1000


class MessageDataReaderListenerImpl : public OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:

    MessageDataReaderListenerImpl(){};

    virtual ~MessageDataReaderListenerImpl(){};

    void on_requested_deadline_missed(DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus & status) override
    {
        std::cerr << "ExchangeEventDataReaderListenerImpl::on_requested_deadline_missed" << std::endl;
    }

    void on_requested_incompatible_qos(DDS::DataReader_ptr reader, const DDS::RequestedIncompatibleQosStatus & status) override
    {
        std::cerr << "ExchangeEventDataReaderListenerImpl::on_requested_incompatible_qos" << std::endl;
    }

    void on_liveliness_changed(DDS::DataReader_ptr reader, const DDS::LivelinessChangedStatus & status) override
    {
        std::cerr << "ExchangeEventDataReaderListenerImpl::on_liveliness_changed" << std::endl;
    }

    void on_subscription_matched(DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus & status) override
    {
        std::cerr << "ExchangeEventDataReaderListenerImpl::on_subscription_matched" << std::endl;
    }

    void on_sample_rejected(DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus& status) override
    {
        std::cerr << "ExchangeEventDataReaderListenerImpl::on_sample_rejected" << std::endl;
    }

    void on_data_available(DDS::DataReader_ptr reader) override
    {
        DDSTopic::msgDataReader_var message_dr = DDSTopic::msgDataReader::_narrow(reader);
        if (CORBA::is_nil(message_dr.in()))
        {
            std::cerr << "ExchangeEventDataReaderListenerImpl::on_data_available: _narrow failed." << std::endl;
            ACE_OS::exit(1);
        }

        int count = 0;
        while (true)
        {
            DDSTopic::msg msg;
            DDS::SampleInfo si;
            DDS::ReturnCode_t status = message_dr->take_next_sample(msg, si);

            if (status == DDS::RETCODE_OK)
            {
                //> Print
                std::cout << "[Recived]  Sender: " <<  msg.sender << "  receiver: " <<  msg.receiver << "  data: "
                    <<  msg.data << "  type: " <<  msg.type << "  timestamp: " <<  msg.timestamp << std::endl;

                ++count;
                break;
            }
            else if (status == DDS::RETCODE_NO_DATA)
            {
                std::cerr << "INFO: reading complete after " << count << " samples." << std::endl;
                break;
            }
            else
            {
                std::cerr << "ERROR: read ExchangeEvent: Error: " << status << std::endl;
            }
        }
    }

    void on_sample_lost(DDS::DataReader_ptr reader, const DDS::SampleLostStatus& status) override
    {
        std::cerr << "ExchangeEventDataReaderListenerImpl::on_sample_lost" << std::endl;
    }
};


int main(int argc, char *argv[])
{
    std::cout << "Running publisher..." << std::endl;


    //> Initialize and create a DomainParticipant
    DDS::DomainParticipant_var participant;
    {
        char config_arg[] = "-DCPSConfigFile";
        char config_file[] = "rtps.ini";
        char * config[] = {config_arg, config_file};
        int config_args = 2;
        DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(config_args, config);
        participant = dpf->create_participant(DDS_DOMAIN, PARTICIPANT_QOS_DEFAULT, DDS::DomainParticipantListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(participant.in()))
        {
            std::cerr << "create_participant failed." << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Create publisher
    DDS::Subscriber_var sub;
    {
        sub = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT, DDS::SubscriberListener::_nil(), ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(sub.in()))
        {
            std::cerr << "create_subscriber failed." << std::endl;
            ACE_OS::exit(1);
        }
    }



    //> Register type of the created idl
    const char * topic_name = "Message";
    const char * topic_type = "Message_t";
    {
        DDSTopic::msgTypeSupport_var msg_servant = new DDSTopic::msgTypeSupportImpl();
        if (DDS::RETCODE_OK != msg_servant->register_type(participant.in(), topic_type))
        {
            std::cerr << "register_type for " << topic_type << " failed." << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Create Topic
    DDS::Topic_var topic;
    {
        topic = participant->create_topic(topic_name, topic_type, TOPIC_QOS_DEFAULT, DDS::TopicListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(topic.in()))
        {
            std::cerr << "create_topic for " << topic_type << " failed." << std::endl;
            ACE_OS::exit(1);
        }
    }



    //> Create a DataReader for the topic_type
    DDS::DataReader_var datareader;
    MessageDataReaderListenerImpl message_in_port;
    {
        DDS::DataReaderListener_var qos_listener = new MessageDataReaderListenerImpl();
        if (CORBA::is_nil(qos_listener.in()))
        {
            std::cerr << "ExchangeEvent listener is nil." << std::endl;
            ACE_OS::exit(1);
        }
        datareader = sub->create_datareader(topic.in(), DATAREADER_QOS_DEFAULT, qos_listener.in(), ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(datareader.in()))
        {
            std::cerr << "msgDataReader could not be created" << std::endl;
            ACE_OS::exit(1);
        }
    }



    //> Wait for events from the Publisher; shut down when "close" received
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }






    std::cout << "Done!" << std::endl;
    return 0;
}