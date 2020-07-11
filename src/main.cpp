//#include <thread>
//#include <chrono>
#include <iostream>


//> IDL gen code
#include "msgC.h"
#include "msgS.h"
#include "msgTypeSupportS.h"
#include "msgTypeSupportC.h"
#include "msgTypeSupportImpl.h"


//> DDS headers
//#include "dds/DdsDcpsPublicationC.h"
//#include "dds/DdsDcpsSubscriptionC.h"
//#include "dds/DdsDcpsDomainC.h"

#include "dds/DCPS/Service_Participant.h"
#include "dds/DCPS/Marked_Default_Qos.h"
//#include "dds/DCPS/PublisherImpl.h"
//#include "ace/streams.h"
//#include "orbsvcs/Time_Utilities.h"


//> Transport and discovery static linking
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#include <dds/DCPS/RTPS/RtpsDiscovery.h>


#define DDS_DOMAIN  1000



int main(int argc, char *argv[])
{
    std::cout << "Running subcriber..." << std::endl;

    


    //> Initialize and create a DomainParticipant
    char config_arg[] = "-DCPSConfigFile";
    char config_file[] = "rtps.ini";
    char * config[] = {config_arg, config_file};
    int config_args = 2;
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(config_args, config);
    DDS::DomainParticipant_var participant = dpf->create_participant(DDS_DOMAIN, PARTICIPANT_QOS_DEFAULT, DDS::DomainParticipantListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (CORBA::is_nil(participant.in()))
    {
        std::cerr << "create_participant failed." << std::endl;
        ACE_OS::exit(1);
    }


    //> Create publisher
    DDS::Publisher_var pub = participant->create_publisher(PUBLISHER_QOS_DEFAULT, DDS::PublisherListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (CORBA::is_nil(pub.in()))
    {
        std::cerr << "create_subscriber failed." << std::endl;
        ACE_OS::exit(1);
    }


    //> Register type of the created idl
    const char * topic_name = "Message";
    const char * topic_type = "Message_t";
    DDSTopic::msgTypeSupport_var msg_servant = new DDSTopic::msgTypeSupportImpl();
    if (DDS::RETCODE_OK != msg_servant->register_type(participant.in(), topic_type))
    {
        std::cerr << "register_type for " << topic_type << " failed." << std::endl;
        ACE_OS::exit(1);
    }


    //> Create Topic
    DDS::Topic_var topic = participant->create_topic(topic_name, topic_type, TOPIC_QOS_DEFAULT, DDS::TopicListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (CORBA::is_nil(topic.in()))
    {
        std::cerr << "create_topic for " << topic_type << " failed." << std::endl;
        ACE_OS::exit(1);
    }



    //> Create a DataWriter for the topic_type
    DDS::DataWriter_var message_base_dw = pub->create_datawriter(topic.in(), DATAWRITER_QOS_DEFAULT, DDS::DataWriterListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (CORBA::is_nil(message_base_dw.in()))
    {
        std::cerr << "create_datawriter for " << topic_name << " failed." << std::endl;
        ACE_OS::exit(1);
    }
    DDSTopic::msgDataWriter_var datawriter = DDSTopic::msgDataWriter::_narrow(message_base_dw.in());
    if (CORBA::is_nil(datawriter.in()))
    {
        std::cerr << "msgDataWriter could not " << "be narrowed" << std::endl;
        ACE_OS::exit(1);
    }


    //std::map<::TAO::String_Manager, DDS::InstanceHandle_t> handles;
    //DDSTopic::msg received_msg;
    //received_msg.exchange = topic_name;
    //auto message_handle = datawriter->register_instance(received_msg);
    //handles[topic_name] = message_handle;






    std::cout << "Done!" << std::endl;
    return 0;
}