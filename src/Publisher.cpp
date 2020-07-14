#include <thread>
#include <chrono>
#include <iostream>


//> IDL gen code
#include "msgTypeSupportImpl.h"


//> Transport and discovery static linking
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#include <dds/DCPS/RTPS/RtpsDiscovery.h>


#define EXAMPLE_DOMAIN  1000
#define EXAMPLE_TOPIC_NAME "Message"



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
        participant = dpf->create_participant(EXAMPLE_DOMAIN, PARTICIPANT_QOS_DEFAULT, DDS::DomainParticipantListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(participant.in()))
        {
            std::cerr << "create_participant failed." << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Create publisher
    DDS::Publisher_var pub;
    {
        pub = participant->create_publisher(PUBLISHER_QOS_DEFAULT, DDS::PublisherListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(pub.in()))
        {
            std::cerr << "create_subscriber failed." << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Register topic type of the created idl
    {
        DDSTopic::msgTypeSupport_var msg_servant = new DDSTopic::msgTypeSupportImpl();
        if (DDS::RETCODE_OK != msg_servant->register_type(participant.in(), DDSTopic::msgTypeName))
        {
            std::cerr << "register_type for " << DDSTopic::msgTypeName << " failed." << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Create Topic
    DDS::Topic_var topic;
    {
        topic = participant->create_topic(EXAMPLE_TOPIC_NAME, DDSTopic::msgTypeName, TOPIC_QOS_DEFAULT, DDS::TopicListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(topic.in()))
        {
            std::cerr << "create_topic for " << DDSTopic::msgTypeName << " failed." << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Create a DataWriter for the DDSTopic::msgTypeName
    DDSTopic::msgDataWriter_var datawriter;
    {
        DDS::DataWriter_var message_base_dw = pub->create_datawriter(topic.in(), DATAWRITER_QOS_DEFAULT, DDS::DataWriterListener::_nil(), OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (CORBA::is_nil(message_base_dw.in()))
        {
            std::cerr << "create_datawriter for " << EXAMPLE_TOPIC_NAME << " failed." << std::endl;
            ACE_OS::exit(1);
        }
        datawriter = DDSTopic::msgDataWriter::_narrow(message_base_dw.in());
        if (CORBA::is_nil(datawriter.in()))
        {
            std::cerr << "msgDataWriter could not " << "be narrowed" << std::endl;
            ACE_OS::exit(1);
        }
    }


    //> Loop
    int counter = 0;
    srand (time(NULL));
    std::string sender = std::string("sender_") + std::to_string(rand() % 1000);
    while (true)
    {
        DDSTopic::msg msg;
        msg.sender = sender.c_str();
        msg.data = std::to_string(counter++).c_str();
        msg.type = 1;
        auto exchange_handle = datawriter->register_instance(msg);
        DDS::ReturnCode_t ret = datawriter->write(msg, exchange_handle);
        if (ret == DDS::RETCODE_OK)
        {
            //> Print
            std::cout << "[Sent] Sender: " <<  msg.sender << "  data: " <<  msg.data << "  type: " <<  msg.type << std::endl;
        }
        else
        {
            std::cerr << "Send error (" << ret << ")" << std::endl;
        }


        //> Wait random time between 2000-50 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1950 + 50));
    }




    std::cout << "Done!" << std::endl;
    return 0;
}