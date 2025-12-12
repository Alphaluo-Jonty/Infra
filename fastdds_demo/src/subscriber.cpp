#include <iostream>
#include <condition_variable>

// FastDDS includes
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/log/Log.hpp>

// Define our data type (should match publisher)
struct HelloWorld
{
    unsigned long index;
    std::string message;
};

// Forward declaration for the type support
class HelloWorldTypeSupport : public eprosima::fastdds::dds::TypeSupport
{
public:
    HelloWorldTypeSupport();
    virtual ~HelloWorldTypeSupport() = default;
    
    bool serialize(void* data, eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;
    bool deserialize(eprosima::fastrtps::rtps::SerializedPayload_t* payload, void* data) override;
    std::function<uint32_t()> getSerializedSizeProvider(void* data) override;
    void* createData() override;
    void deleteData(void* data) override;
    bool getKey(void* data, eprosima::fastrtps::rtps::InstanceHandle_t* ihandle, bool force_md5 = false) override;
};

using namespace eprosima::fastdds::dds;

// Subscriber Listener
class SubListener : public DataReaderListener
{
public:
    SubListener()
        : matched_(0)
    {
    }

    ~SubListener() override
    {
    }

    void on_data_available(
            DataReader* reader) override
    {
        HelloWorld st;

        SampleInfo info;
        if (reader->take_next_sample(&st, &info) == ReturnCode_t::RETCODE_OK)
        {
            if (info.valid_data)
            {
                std::cout << "Sample received (index=" << st.index << ", message=\"" << st.message << "\")" << std::endl;
            }
        }
    }

    void on_subscription_matched(
            DataReader*,
            const SubscriptionMatchedStatus& info) override
    {
        if (info.current_count_change == 1)
        {
            matched_ = info.total_count;
            std::cout << "Subscriber matched with publisher" << std::endl;
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.total_count;
            std::cout << "Subscriber unmatched from publisher" << std::endl;
        }
        else
        {
            std::cout << info.current_count_change 
                      << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
        }
    }

    std::atomic_int matched_;
};

SubListener g_sublistener_;

int main(int argc, char** argv)
{
    // Create Participant
    DomainParticipantQos participant_qos;
    participant_qos.name("Participant_subscriber");
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

    // Create Subscriber
    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    if (subscriber == nullptr) {
        std::cout << "Create subscriber failed." << std::endl;
        return -1;
    }

    // Create DataReader
    DataReader* reader = subscriber->create_datareader(topic, DATAREADER_QOS_DEFAULT, &g_sublistener_);
    if (reader == nullptr) {
        std::cout << "Create datareader failed." << std::endl;
        return -1;
    }

    std::cout << "Subscriber running... Press Enter to stop." << std::endl;
    
    // Wait for user input to stop
    std::string input;
    std::getline(std::cin, input);

    // Delete entities
    subscriber->delete_datareader(reader);
    participant->delete_subscriber(subscriber);
    participant->delete_topic(topic);
    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return 0;
}