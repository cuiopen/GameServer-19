
#include "Config.h"
#include "Encoding_POSIX.h"
#include <errno.h>

namespace buf
{

#define E2BIG_SIZE 256

bool EncodingImpl::openImpl(const char* fromcode, const char* tocode)
{
    if (_converter != (encoder_type)-1)
        closeImpl();
    _converter = iconv_open(tocode, fromcode);
    if (_converter != (encoder_type)-1)
    {
        encodingImpl(0, 0, 0, 0);
        return true;
    }
    return false;
}

EncodingImpl::size_type EncodingImpl::encodingImpl(
        const char* ibuf,
        size_type ibufsize,
        char* obuf,
        size_type obufsize)
{
    if (!ibuf || !ibufsize || !obuf || !obufsize)
        return 0;
    if (_converter == (encoder_type)-1)
        return 0;

    const char** pibuf = &ibuf;
    size_type* pibytesleft = &ibufsize;
    char** pobuf = &obuf;
    size_type* pobytesleft = &obufsize;
    size_type obufsizemax = obufsize;
    size_type ret = 0;

#ifdef __FreeBSD__
    ret = ::iconv(_converter, pibuf, pibytesleft, pobuf, pobytesleft);
#else
    ret = ::iconv(_converter, (char**)pibuf, pibytesleft, pobuf, pobytesleft);
#endif
    if (ret == ((size_type)-1)
            && (errno == E2BIG
            || errno == EILSEQ
            || errno == EINVAL))
        return 0;

    return obufsizemax - obufsize;
}

EncodingImpl::size_type EncodingImpl::encodingImpl(const char* ibuf, size_type ibufsize)
{
    if (!ibuf || !ibufsize)
        return 0;

    if (_converter == (encoder_type)-1)
        return 0;

    clearImpl();

    const char** pibuf = &ibuf;
    size_type* pibytesleft = &ibufsize;

    size_type ret = 0;
    size_type size = 0;
    size_type bufsize = 0;
    std::vector<char>::pointer buf = 0;
    _buffer.resize((ibufsize)<<1);
    while (true)
    {
        buf = &_buffer[0];
        bufsize = _buffer.capacity();
        size = bufsize;
#ifdef __FreeBSD__
        ret = ::iconv(_converter, pibuf, pibytesleft, &buf, &size);
#else
        ret = ::iconv(_converter, (char**)pibuf, pibytesleft, &buf, &size);
#endif
        if (ret == ((size_type)-1)
                && (errno == E2BIG
                || errno == EILSEQ
                || errno == EINVAL))
        {
            if (errno == E2BIG)
                _buffer.resize(_buffer.capacity()+E2BIG_SIZE);
            else
                return 0;
        }
        _datasize = bufsize - size;
        return _datasize;
    }
    return 0;
}

void EncodingImpl::closeImpl()
{
    if (_datasize)
    {
        _buffer.clear();
        _datasize = 0;
    }
    if (_converter != (encoder_type)-1)
        iconv_close(_converter);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

