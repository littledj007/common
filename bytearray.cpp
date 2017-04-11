#include "bytearray.h"
#include <cassert>
#include <string>


namespace gcommon
{
    ByteArray::ByteArray(const uint32_t capacity)
    {
        if (capacity == 0)
        {
            new(this) ByteArray(1);
            return;
        }

        this->m_buff = new uint8_t[capacity + 1];
        if (this->m_buff)
        {
            memset(this->m_buff, 0, capacity + 1);
            this->m_size = 0;
            this->m_capacity = capacity;
        }
        else
        {
            this->m_size = 0;
            this->m_capacity = 0;
        }
    }

    ByteArray::ByteArray(const uint32_t capacity, const uint8_t pattern)
    {
        if (capacity == 0)
        {
            new(this) ByteArray(1);
            return;
        }

        this->m_buff = new uint8_t[capacity + 1];
        if (this->m_buff)
        {
            memset(this->m_buff, 0, capacity + 1);
            memset(this->m_buff, pattern, capacity);
            this->m_capacity = capacity;
            this->m_size = capacity;
        }
        else
        {
            this->m_capacity = 0;
            this->m_size = 0;
        }
    }

    ByteArray::ByteArray(const void * array, const
        uint32_t size, const uint32_t capacity)
    {
        if (capacity == 0)
        {
            new(this) ByteArray(1);
            return;
        }

        if (array == NULL || size == 0)
        {
            new(this) ByteArray(capacity);
            return;
        }

        uint32_t newsize = (size > capacity) ? capacity : size;
        this->m_buff = new uint8_t[capacity + 1];
        if (this->m_buff)
        {
            memset(this->m_buff, 0, capacity + 1);
            memcpy(this->m_buff, array, newsize);
            this->m_size = newsize;
            this->m_capacity = capacity;
        }
        else
        {
            this->m_size = 0;
            this->m_capacity = 0;
        }
    }


    ByteArray::ByteArray(const void * array, const uint32_t size)
    {
        if (array == NULL || size == 0)
        {
            new(this) ByteArray(1);
            return;
        }

        this->m_buff = new uint8_t[size + 1];
        if (this->m_buff)
        {
            memset(this->m_buff, 0, size + 1);
            memcpy(this->m_buff, array, size);
            this->m_size = size;
            this->m_capacity = size;
        }
        else
        {
            this->m_size = 0;
            this->m_capacity = 0;
        }
    }

    ByteArray::ByteArray(const char* array)
    {
        uint32_t size = strlen(array);
        if (array == NULL || size == 0)
        {
            new(this) ByteArray(1);
            return;
        }

        this->m_buff = new uint8_t[size + 1];
        if (this->m_buff)
        {
            memset(this->m_buff, 0, size + 1);
            memcpy(this->m_buff, array, size);
            this->m_size = size;
            this->m_capacity = size;
        }
        else
        {
            this->m_size = 0;
            this->m_capacity = 0;
        }
    }

    ByteArray::ByteArray(const ByteArray & other)
    {
        uint32_t size = other.size();
        uint32_t capacity = other.capacity();
        size = (size > capacity) ? capacity : size;

        if (other.getRawPointer() == NULL || size == 0)
        {
            new(this) ByteArray(capacity);
            return;
        }

        this->m_buff = new uint8_t[capacity + 1];
        if (this->m_buff)
        {
            memset(this->m_buff, 0, capacity + 1);
            memcpy(this->m_buff, other.getRawPointer(), size);

            this->m_size = size;
            this->m_capacity = capacity;
        }
        else
        {
            this->m_size = 0;
            this->m_capacity = 0;
        }
    }

    ByteArray::~ByteArray()
    {
        if (this->m_buff)
            delete[] this->m_buff;
        this->m_buff = NULL;
    }

    uint8_t * ByteArray::getRawPointer() const
    {
        return this->m_buff;
    }

    bool ByteArray::operator==(const ByteArray &other) const
    {
        if (this->m_buff == NULL || this->m_capacity == 0)
        {
            return true;
        }

        if (this->size() != other.size())
            return false;

        if (memcmp(this->m_buff, other.m_buff, this->size()) != 0)
            return false;

        return true;
    }

    bool ByteArray::operator<(const ByteArray &other) const
    {
        if (this->m_buff == NULL || this->m_capacity == 0)
        {
            return false;
        }

        uint32_t minimal_size =
            (this->size() < other.size()) ? this->size() : other.size();

        if (memcmp(this->m_buff, other.m_buff, minimal_size) < 0)
            return true;
        else if (memcmp(this->m_buff, other.m_buff, minimal_size) > 0)
            return false;
        else
            return (this->size() < other.size());
    }

    bool ByteArray::operator>(const ByteArray &other) const
    {
        if (this->m_buff == NULL || this->m_capacity == 0)
        {
            return false;
        }

        uint32_t minimal_size =
            (this->size() < other.size()) ? this->size() : other.size();

        if (memcmp(this->m_buff, other.m_buff, minimal_size) < 0)
            return false;
        else if (memcmp(this->m_buff, other.m_buff, minimal_size) > 0)
            return true;
        else
            return (this->size() > other.size());
    }

    uint8_t & ByteArray::operator [ ] (const uint32_t position) const
    {
        if (this->m_buff == NULL ||
            this->m_capacity == 0 ||
            position >= this->m_size)
        {
            uint8_t* tmp = new uint8_t[0];
            *tmp = 0;
            return *tmp;
        }

        return this->m_buff[position];
    }

    auto_ptr< ByteArray > ByteArray::clone() const
    {
        auto_ptr<ByteArray> result(new ByteArray(*this));
        return result;
    }

    uint32_t ByteArray::size() const
    {
        return this->m_size;
    }

    void ByteArray::setSize(const uint32_t size)
    {
        if (size > this->m_capacity)
            this->m_size = this->m_capacity;
        else
            this->m_size = size;
    }

    uint32_t ByteArray::capacity() const
    {
        return this->m_capacity;
    }


}
