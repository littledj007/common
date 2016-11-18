#pragma once

#include <cstdint>
#include <memory>

using namespace std;

namespace gcommon 
{
    class ByteArray 
	{
        private:
            uint8_t* m_buff;       // Internal pointer to the begin of the array 
			uint32_t m_capacity;
			uint32_t m_size;
			
        public:
			ByteArray(const uint32_t capacity);
			ByteArray(const uint32_t capacity, const uint8_t pattern);
			ByteArray(const void * array, const uint32_t size, const uint32_t capacity);
			ByteArray(const void * array, const uint32_t size);
			ByteArray(const char* array);
            ByteArray(const ByteArray& other);		
			virtual ~ByteArray();
			
            virtual uint8_t* getRawPointer() const; 
			virtual bool operator==(const  ByteArray &other) const;
			virtual bool operator<(const  ByteArray &other) const;
			virtual bool operator>(const  ByteArray &other) const;
			virtual uint8_t& operator[] (const  uint32_t position) const;
			virtual auto_ptr<ByteArray> clone() const;
            virtual uint32_t size() const;
			virtual uint32_t capacity() const;
			virtual void setSize(const  uint32_t size);
    };
}

