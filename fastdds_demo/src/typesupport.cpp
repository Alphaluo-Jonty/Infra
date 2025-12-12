#include "typesupport.hpp"
#include <fastcdr/Cdr.h>
#include <fastcdr/exceptions/BadParamException.h>
#include <fastrtps/types/TypeIdentifier.h>
#include <fastrtps/types/TypeObject.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/types/DynamicDataFactory.h>

HelloWorldTypeSupport::HelloWorldTypeSupport()
{
    // Set type name
    setName("HelloWorld");
    m_typeSize = 4096; // Set maximum serialized size
    m_isGetKeyDefined = false; // We don't use keys in this example
}

bool HelloWorldTypeSupport::serialize(
        void* data,
        eprosima::fastrtps::rtps::SerializedPayload_t* payload)
{
    HelloWorld* p_type = static_cast<HelloWorld*>(data);

    // Serialize using FastCDR
    eprosima::fastcdr::FastBuffer fastbuffer(
        reinterpret_cast<char*>(payload->data),
        payload->max_size);

    eprosima::fastcdr::Cdr ser(
        fastbuffer,
        eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
        eprosima::fastcdr::CdrVersion::XCDRv1);
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        ser << p_type->index;
        ser << p_type->message;
    }
    catch (eprosima::fastcdr::exception::BadParamException& /*exception*/)
    {
        return false;
    }

    payload->length = ser.get_serialized_data_length();
    return true;
}

bool HelloWorldTypeSupport::deserialize(
        eprosima::fastrtps::rtps::SerializedPayload_t* payload,
        void* data)
{
    HelloWorld* p_type = static_cast<HelloWorld*>(data);

    eprosima::fastcdr::FastBuffer fastbuffer(
        reinterpret_cast<char*>(payload->data),
        payload->length);

    eprosima::fastcdr::Cdr deser(
        fastbuffer,
        eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
        eprosima::fastcdr::CdrVersion::XCDRv1);

    try
    {
        deser >> p_type->index;
        deser >> p_type->message;
    }
    catch (eprosima::fastcdr::exception::BadParamException& /*exception*/)
    {
        return false;
    }

    return true;
}

std::function<uint32_t()> HelloWorldTypeSupport::getSerializedSizeProvider(void* data)
{
    return [data]() -> uint32_t
    {
        HelloWorld* p_type = static_cast<HelloWorld*>(data);
        eprosima::fastcdr::FastBuffer fastbuffer;
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN, eprosima::fastcdr::CdrVersion::XCDRv1);
        ser << p_type->index;
        ser << p_type->message;
        return static_cast<uint32_t>(ser.get_serialized_data_length());
    };
}

void* HelloWorldTypeSupport::createData()
{
    return new HelloWorld();
}

void HelloWorldTypeSupport::deleteData(void* data)
{
    delete static_cast<HelloWorld*>(data);
}

bool HelloWorldTypeSupport::getKey(
        void* /*data*/,
        eprosima::fastrtps::rtps::InstanceHandle_t* /*ihandle*/,
        bool /*force_md5*/)
{
    // We don't use keys in this example
    return false;
}