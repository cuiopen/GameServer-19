
#ifndef ENCODING_POSIX_H_
#define ENCODING_POSIX_H_

#include <vector>
#include <iconv.h>

namespace buf
{

class EncodingImpl
{
public:
    typedef std::vector<char> container;
    typedef iconv_t encoder_type;
    typedef container::size_type size_type;

public:
    EncodingImpl() : _converter((encoder_type)-1), _buffer(), _datasize(0) {}
    ~EncodingImpl() { closeImpl(); }

    bool openImpl(const char* fromcode, const char* tocode);
    void closeImpl();

    size_type encodingImpl(
            const char* ibuf,
            size_type ibufsize,
            char* obuf,
            size_type obufsize);

    size_type encodingImpl(const char* ibuf, size_type ibufsize);

    inline std::vector<char>& dataImpl() { return _buffer; }
    inline const std::vector<char>& dataImpl() const { return _buffer; }
    inline size_type sizeImpl() const { return _datasize; }

    inline void clearImpl()
    {
        if (_datasize)
            _buffer[0] = '\0';
        _datasize = 0;
    }

private:
    encoder_type _converter;

    std::vector<char> _buffer;
    size_type _datasize;
};

} // namespace buf

#endif // ENCODING_POSIX_H_ 

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

