
#ifndef ZIPPER_H_
#define ZIPPER_H_

#include "Config.h"
#include "Zip.h"

namespace buf
{

class Zipper
{
public:
    typedef std::vector<u8_t> container;
    typedef container::size_type size_type;

public:
    Zipper() {}

    Zipper(const void* data, size_type size)
    {
        zipper(data, size);
    }

    ~Zipper()
    {
        if (_size || _data.size())
            _data.clear();
    }

    inline size_type zipper(const void* data, size_type size)
    {
        if (_size || _data.size())
            _data.clear();
        _size = _zip.zip(_data, reinterpret_cast<Zip::const_pointer>(data), size, Zip::LEVEL_BEST);
        return _size;
    }

    inline const u8_t* operator&() const { return &_data.front(); }
    inline size_type size() const { return _size; } 

private:
    Zip _zip;
    container _data;
    size_type _size;
};

} // namespace buf

#endif // ZIPPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

