
#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_

#include "Config.h"
#include "TypeTraits.h"
#include "Buffer.h"

#include <vector>
#include <string>

namespace buf
{

class ByteBuffer : public Buffer
{
public:
    ByteBuffer() {}
    ByteBuffer(const ByteBuffer& s) : Buffer(s) {}
    ByteBuffer(const_pointer data, size_type size) : Buffer(data, size) {}

    inline const char* operator&()
    {
        return reinterpret_cast<const char*>(rbuf());
    }
};

} // namespace buf

#endif // BYTEBUFFER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

