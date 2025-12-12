#ifndef HELLOWORLD_TYPESUPPORT_HPP
#define HELLOWORLD_TYPESUPPORT_HPP

#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/rtps/common/InstanceHandle.h>
#include <fastcdr/cdr/fixed_size_string.hpp>

#include <string>

// HelloWorld data type
struct HelloWorld
{
    unsigned long index;
    std::string message;
};

// Include the generated TypeObject files for FastDDS
#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>

// A proper TypeSupport implementation for HelloWorld
class HelloWorldTypeSupport : public eprosima::fastdds::dds::TypeSupport
{
public:
    HelloWorldTypeSupport();
    virtual ~HelloWorldTypeSupport() = default;

    bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    void* createData() override;

    void deleteData(
            void* data) override;

    bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;
};

#endif // HELLOWORLD_TYPESUPPORT_HPP