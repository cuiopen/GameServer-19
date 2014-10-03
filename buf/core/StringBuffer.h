
#ifndef STRINGBUFFER_H_
#define STRINGBUFFER_H_

#include "Config.h"
#include "Utils.h"
#include "Buffer.h"

#include <vector>
#include <string>

namespace buf
{

class StringBuffer : public Buffer
{
public:
    StringBuffer() {}
    StringBuffer(const StringBuffer& s) : Buffer(s) {}
    StringBuffer(const_pointer data, size_type size) : Buffer(data, size) {}

    template <typename T>
    inline StringBuffer& operator<<(const T& v)
    {
        char buffer[32] = {0};
        int len = num2str(v, buffer, sizeof(buffer));
        buffer[len] = '\0';
        append(reinterpret_cast<const_pointer>(buffer), len);
        return *this;
    }

    inline const char* operator&()
    {
        return reinterpret_cast<const char*>(rbuf());
    }

    inline StringBuffer& operator<<(const char* str)
    {
        if (!str)
            return *this;
        size_type len = strlen(str);
        append(reinterpret_cast<const_pointer>(str), len);
        return *this;
    }

    inline StringBuffer& operator<<(const std::string& str)
    {
        if (!str.length())
            return *this;
        return operator<<(str.c_str());
    }
};

} // namespace buf

#endif // STRINGBUFFER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

