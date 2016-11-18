#pragma once

#include "bytearray.h"
#include <cstdint>
#include <memory>

using namespace std;
using namespace gcommon;

namespace gcommon 
{
    class ByteBuffer : public ByteArray
	{
        private:
			uint32_t m_readPos;
			uint32_t m_writePos;

        public:
            ByteBuffer( uint32_t capacity );
            ByteBuffer( const ByteBuffer& other );
            ByteBuffer( const ByteArray& byte_array );			
			int writeBuffer(const void* buffer, uint32_t buffer_size);
			int writeByteArray(const ByteArray& byte_array);
			int writeInt8(uint8_t value);
			int writeInt16(uint16_t value);
			int writeInt32(uint32_t value);
			int fillBytes(uint32_t num_bytes, uint8_t byte);
            int readBuffer( uint32_t size_to_read, void* destination_buffer );
            auto_ptr<ByteArray> readByteArray( uint32_t size );
            uint8_t readInt8();
            uint16_t readInt16();
            uint32_t readInt32();
			uint32_t getWritePosition() const;
            void setWritePosition( uint32_t position );
			uint32_t getReadPosition() const;
            void setReadPosition( uint32_t position );
            void reset();
            void skip( uint32_t bytes );

			~ByteBuffer();
    };
}