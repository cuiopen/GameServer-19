
#ifndef MAPLOADER_H_
#define MAPLOADER_H_

#include "core/Buffer.h"
#include "core/Utils.h"

namespace buf
{

union Magic
{
    char BUFM[4];
    u32_t bufm;
} bufm;

template <typename T>
class MapLoader
{
public:
    MapLoader(const char* file)
    {
        MEMZRO(_file, sizeof(_file));
        cpystr(file, _file, sizeof(_file));
        bufm.BUFM[0] = 'B';
        bufm.BUFM[1] = 'U';
        bufm.BUFM[2] = 'F';
        bufm.BUFM[3] = 'M';
    }
    virtual ~MapLoader()
    {
        _data.clear();
    }

    virtual bool load(bool force = false)
    {
        /*
         * MAGIC:4 => BUFM
         * WIDTH:4
         * HEIGHT:4
         * T[WIDTH*HEIGHT]
         **/

        if (!strlen(_file))
            return false;

        File f(_file);
        if (!f.open())
        {
            // TODO: log
            return false;
        }

        if (_loaded)
        {
            if (force)
                clear();
            else
                return false;
        }

        u32_t magic = 0;
        u32_t size = 0;
        u32_t width = 0;
        u32_t height = 0;
        u32_t total = 0;

        int ret = f.read(&magic, sizeof(magic));
        if (ret < (int)sizeof(magic))
            goto _err;

        if (magic != bufm.bufm)
            goto _err;

        ret = f.read(&width, sizeof(width));
        if (ret < (int)sizeof(width))
            goto _err;

        ret = f.read(&height, sizeof(height));
        if (ret < (int)sizeof(height))
            goto _err;

        total = width*height;
        if (!total)
            goto _err;

        size = total*sizeof(T);
        _data.resize(size);
        ret = f.read(_data.wbuf(), size);
        if (ret < (int)size)
        {
            clear();
            goto _err;
        }
        _data.wflip(ret);

        _loaded = true;
        f.close();
        return true;

    _err:
        f.close();
        return false;
    }

    bool reload()
    {
        clear();
        return load();
    }

    inline const Buffer& getData() const { return _data; }
    inline Buffer& getData() { return _data; }
    inline size_t getSize() const { return _data.rsize(); }

    inline u32_t width() const { return _width; }
    inline u32_t height() const { return _height; }

    inline bool isLoaded() const { return _loaded; }

    inline size_t size() const { return (_data.rsize()+sizeof(T))/sizeof(T); }

    void clear()
    {
        _data.clear();
        _loaded = false;
    }

private:
    char _file[PATH_MAX+1];
    bool _loaded;
    u32_t _width;
    u32_t _height;
    Buffer _data;
};

} // namespace buf

#endif // MAPLOADER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

