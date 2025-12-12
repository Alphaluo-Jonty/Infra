#include <iostream>
#include <chrono>
#include <thread>

// FastDDS includes
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/log/Log.hpp>

// Include our type support
#include "typesupport.hpp"

using namespace eprosima::fastdds::dds;

// Publisher Listener
class PubListener : public DataWriterListener
{
public:
    PubListener()
        : matched_(0)
    {
    }

    ~PubListener() override
    {
    }

    void on_publication_matched(
            DataWriter*,
            const PublicationMatchedStatus& info) override
    {
        if (info.current_count_change == 1)
        {
            matched_ = info.total_count;
            std::cout << "Publisher matched with subscriber" << std::endl;
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.total_count;
            std::cout << "Publisher unmatched from subscriber" << std::endl;
        }
        else
        {
            std::cout << info.current_count_change 
                      << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
        }
    }

    std::atomic_int matched_;
};

PubListener pub_listener_;

int main(int argc, char** argv)
{
    // Initialize sample
    int samples = 10;
    if (argc > 1) {
        samples = std::atoi(argv[1]);
    }

    // Create Participant
    DomainParticipantQos participant_qos;
    participant_qos.name("Participant_publisher");
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, participant_qos);
    
    if (participant == nullptr) {
        std::cout << "Create participant failed." << std::endl;
        return -1;
    }

    // Register the Type
    HelloWorldTypeSupport type_support;
    participant->register_type(type_support, "HelloWorld");

    // Create Topic
    Topic* topic = participant->create_topic("HelloWorldTopic", "HelloWorld", TOPIC_QOS_DEFAULT);
    if (topic == nullptr) {
        std::cout << "Create topic failed." << std::endl;
        return -1;
    }

    // Create Publisher
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    if (publisher == nullptr) {
        std::cout << "Create publisher failed." << std::endl;
        return -1;
    }

    // Create DataWriter
    DataWriter* writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT, &pub_listener_);
    if (writer == nullptr) {
        std::cout << "Create datawriter failed." << std::endl;
        return -1;
    }

    std::cout << "Publisher running for " << samples << " samples..." << std::endl;
    
    // Main publishing loop
    HelloWorld st;
    /* Initialize your structure here */
    int msgsent = 0;
    while (msgsent < samples)
    {
        st.index = msgsent++;
        st.message = "HelloWorld " + std::to_string(st.index);
        
        std::cout << "Sending sample, index: " << st.index << ", message: \"" << st.message << "\"" << std::endl;
        writer->write(&st);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Wait for discovery
    std::cout << "Waiting for 5 seconds to discover subscribers..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Delete entities
    publisher->delete_datawriter(writer);
    participant->delete_publisher(publisher);
    participant->delete_topic(topic);
    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return 0;
}