
#ifndef ENCODING_H_
#define ENCODING_H_

#include "Config.h"
#ifdef UNIXLIKE
#include "linux/Encoding_POSIX.h"
#endif

namespace buf
{

class Encoding : public EncodingImpl
{
public:
    Encoding() {}
    ~Encoding() {}

    inline bool open(const char* fromcode, const char* tocode)
    {
        return openImpl(fromcode, tocode);
    }

    inline void close() { closeImpl(); }

    inline size_type encoding(
            const char* ibuf,
            size_type ibufsize,
            char* obuf,
            size_type obufsize)
    {
        return encodingImpl(ibuf, ibufsize, obuf, obufsize);
    }

    inline size_type encoding(const char* ibuf, size_type ibufsize)
    {
        return encodingImpl(ibuf, ibufsize);
    }

    inline const std::vector<char>& data() const { return dataImpl(); }
    inline std::vector<char>& data() { return dataImpl(); }

    inline size_type size() const { return sizeImpl(); }
    inline void clear() { clearImpl(); }
};

} // namespace buf

#endif // ENCODING_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

