#include "bytebuffer.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cassert>
#include "common.h"

namespace gcommon
{
    ByteBuffer::ByteBuffer(uint32_t capacity) : ByteArray(capacity)
    {
        this->m_readPos = 0;
        this->m_writePos = 0;
    }

    ByteBuffer::ByteBuffer(const ByteBuffer & other) : ByteArray(other)
    {
        this->m_readPos = other.getReadPosition();
        this->m_writePos = other.getWritePosition();
    }

    ByteBuffer::ByteBuffer(const ByteArray & byte_array) : ByteArray(byte_array)
    {
        this->m_readPos = 0;
        this->m_writePos = 0;
    }

    ByteBuffer::~ByteBuffer()
    {
    }

    int ByteBuffer::writeBuffer(const void * buffer, uint32_t buffer_size)
    {
        // No real write when this ocurrs
        if (buffer_size == 0 || buffer == NULL)
            return -1;

        // If more memory is needed
        if (this->m_writePos + buffer_size >= this->capacity())
            return -1;

        // Copy the buffer at the end of the array
        memcpy(this->getRawPointer() + this->m_writePos, buffer, buffer_size);        

        // Increments the write position        
        this->m_writePos += buffer_size;
        this->setSize(this->m_writePos);

        return buffer_size;
    }

    int ByteBuffer::writeByteArray(const ByteArray & byte_array)
    {
        return this->writeBuffer(byte_array.getRawPointer(), byte_array.size());
    }

    int ByteBuffer::writeInt8(uint8_t value)
    {
        return this->writeBuffer(&value, 1);
    }

    int ByteBuffer::writeInt16(uint16_t value)
    {
        uint16_t temp = g_htons(value);
        return this->writeBuffer((uint8_t*)& temp, 2);
    }

    int ByteBuffer::writeInt32(uint32_t value)
    {
        uint32_t temp = g_htonl(value);
        return this->writeBuffer((uint8_t*)& temp, 4);
    }

    int ByteBuffer::fillBytes(uint32_t num_bytes, uint8_t byte)
    {
        // If more memory is needed
        if (this->m_writePos + num_bytes >= this->capacity())
            return -1;

        memset(this->getRawPointer() + this->m_writePos, byte, num_bytes);
        this->m_writePos += num_bytes;

        return num_bytes;
    }

    int ByteBuffer::readBuffer(uint32_t size_to_read, void * buffer)
    {
        if (buffer == NULL)
        {
            return 0;
        }

        if (this->m_readPos + size_to_read >= this->capacity())
            return 0;

        memcpy(buffer, this->getRawPointer() + this->m_readPos, size_to_read);

        this->m_readPos += size_to_read;
        return size_to_read;
    }

    auto_ptr<ByteArray> ByteBuffer::readByteArray(uint32_t size)
    {
        auto_ptr<ByteArray> result(new ByteArray(size));
        this->readBuffer(size, result->getRawPointer());

        return result;
    }

    uint8_t ByteBuffer::readInt8()
    {
        uint8_t temp = 0;
        this->readBuffer(1, (uint8_t*)& temp);
        return temp;
    }

    uint16_t ByteBuffer::readInt16()
    {
        uint16_t temp = 0;
        this->readBuffer(2, (uint8_t*)& temp);
        return g_ntohs(temp);
    }

    uint32_t ByteBuffer::readInt32()
    {
        uint32_t temp = 0;
        this->readBuffer(4, (uint8_t*)& temp);
        return g_ntohl(temp);
    }

    uint32_t ByteBuffer::getWritePosition() const
    {
        return this->m_writePos;
    }

    uint32_t ByteBuffer::getReadPosition() const
    {
        return this->m_readPos;
    }

    void ByteBuffer::setWritePosition(uint32_t position)
    {
        if (position > this->capacity())
            return;

        this->m_writePos = position;
        this->setSize(this->m_writePos);
    }

    void ByteBuffer::setReadPosition(uint32_t position)
    {
        if (position > this->m_writePos)
            return;

        this->m_readPos = position;
    }

    void ByteBuffer::reset()
    {
        this->m_readPos = 0;
        this->m_writePos = 0;
        this->setSize(0);
    }

    void ByteBuffer::skip(uint32_t bytes)
    {
        if (this->m_readPos + bytes > this->m_writePos)
            return;

        this->m_readPos += bytes;
    }

}
