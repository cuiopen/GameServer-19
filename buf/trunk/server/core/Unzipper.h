
#ifndef UNZIPPER_H_
#define UNZIPPER_H_

#include "Config.h"
#include "Zip.h"

namespace buf
{

class Unzipper
{
public:
    typedef std::vector<u8_t> container;
    typedef container::size_type size_type;
public:
    Unzipper() {}

    Unzipper(const char* data, size_type size)
    {
        unzipper(data, size);
    }

    Unzipper(const u8_t* data, size_type size)
    {
        unzipper(data, size);
    }

    inline size_type unzipper(const char* data, size_type size)
    {
        if (_size || _data.size())
            _data.clear();
        _size = _zip.unzip(_data, reinterpret_cast<Zip::const_pointer>(data), size);
        return _size;
    }

    inline size_type unzipper(const u8_t* data, size_type size)
    {
        if (_size || _data.size())
            _data.clear();
        _size = _zip.unzip(_data, data, size);
        return _size;
    }

    ~Unzipper()
    {
        if (_size)
            _data.clear();
    }

    inline const u8_t* operator & () const { return &_data[0]; }
    inline size_type size() const { return _size; } 

private:
    Zip _zip;
    container _data;
    size_type _size;
};

} // namespace buf

#endif // UNZIPPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

