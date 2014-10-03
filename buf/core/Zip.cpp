
#include "Config.h"
#include "Zip.h"

namespace buf
{

Zip::Zip(const_pointer dict, size_type len)
    : _dicton(false), _adler(0)
{
    if (dict && len)
    {
        _dict.insert(_dict.begin(), dict, dict+len);
        _dicton = true;
    }
}

Zip::~Zip()
{}

Zip::size_type Zip::zip(std::vector<data_type>& dst,
        const_pointer src, size_type srclen, leveltype_e level)
{
    if (!src || !srclen)
        return 0;

    _stream.zalloc = 0;
    _stream.zfree = 0;
    _stream.opaque = 0;

    int ret = Z_OK;
    if (level > LEVEL_BEST || level <= LEVEL_NO)
        ret = deflateInit(&_stream, LEVEL_DEFAULT/*LEVEL_6*/);
    else
        ret = deflateInit(&_stream, level);

    if (ret != Z_OK)
        return 0;

    if (_dicton)
    {
        if (_dict.size() && deflateSetDictionary(&_stream,
                    &_dict.front(), _dict.size()) != Z_OK)
            return 0;
        _adler = _stream.adler;
    }

    size_type size = deflateBound(&_stream, srclen);
    dst.resize(size);

    _stream.next_in = const_cast<pointer>(src);
    _stream.avail_in = srclen;

    while (_stream.avail_in)
    {
        size_type size = dst.size();
        _stream.next_out = &dst[_stream.total_out];
        _stream.avail_out = size-_stream.total_out;

        ret = deflate(&_stream, Z_FINISH);
        if (ret == Z_STREAM_END || ret == Z_MEM_ERROR)
            break; 

        if (ret == Z_BUF_ERROR || !_stream.avail_out)
        {
            dst.resize(size << 1);
        }
        else if (ret == Z_ERRNO
                || ret == Z_STREAM_ERROR
                || ret == Z_DATA_ERROR
                || ret == Z_VERSION_ERROR)
        {
            deflateEnd(&_stream);
            return 0;
        }
    }

    deflateEnd(&_stream);
    dst.resize(_stream.total_out);
    return _stream.total_out;
}

Zip::size_type Zip::unzip(
        std::vector<data_type>& dst, const_pointer src, size_type srclen)
{
    if (!src || !srclen)
        return 0;

    _stream.zalloc = 0;
    _stream.zfree = 0;
    _stream.opaque = 0;

    if (inflateInit(&_stream) != Z_OK)
        return 0;

#ifndef _NDEBUG
    dst.resize(srclen << 1);
#else
    dst.resize(srclen << 2);
#endif

    _stream.next_in = const_cast<pointer>(src);
    _stream.avail_in = srclen;

    int ret = Z_OK;
    while (_stream.avail_in)
    {
        size_type size = dst.size();
        _stream.next_out = &dst[_stream.total_out];
        _stream.avail_out = size-_stream.total_out;

        ret = inflate(&_stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END || ret == Z_MEM_ERROR)
            break; 

        if (ret == Z_BUF_ERROR || !_stream.avail_out)
        {
#ifndef _NDEBUG
            dst.resize(size << 1);
#else
            dst.resize(size << 2);
#endif
        }
        else if (ret == Z_NEED_DICT)
        {
            if (_dicton)
            {
                if (_dict.size() && inflateSetDictionary(&_stream,
                            &_dict[0], _dict.size()) != Z_OK)
                    return 0;
                if (_stream.adler != _adler)
                    return 0;
            }
        } else if (ret == Z_ERRNO
                || ret == Z_STREAM_ERROR
                || ret == Z_DATA_ERROR
                || ret == Z_VERSION_ERROR)
        {
            inflateEnd(&_stream);
            return 0;
        }
    }

    inflateEnd(&_stream);
    dst.resize(_stream.total_out);
    return _stream.total_out;
}

Zip::size_type Zip::zip(pointer* dst, const_pointer src, size_type srclen, leveltype_e level)
{
    if (!dst || !src || !srclen)
        return 0;

    _stream.zalloc = 0;
    _stream.zfree = 0;
    _stream.opaque = 0;

    int ret = Z_OK;
    if (level > LEVEL_BEST || level <= LEVEL_NO)
        ret = deflateInit(&_stream, LEVEL_DEFAULT/*LEVEL_6*/);
    else
        ret = deflateInit(&_stream, level);

    if (ret != Z_OK)
        return 0;

    if (_dicton)
    {
        if (_dict.size() && deflateSetDictionary(&_stream,
                    &_dict.front(), _dict.size()) != Z_OK)
        {
            deflateEnd(&_stream);
            return 0;
        }
        _adler = _stream.adler;
    }

    size_type size = deflateBound(&_stream, srclen);
    *dst = zipalloc(size);
    if (!*dst)
    {
        deflateEnd(&_stream);
        return 0;
    }

    _stream.next_in = const_cast<pointer>(src);
    _stream.avail_in = srclen;

    while (_stream.avail_in)
    {
        _stream.next_out = &(*dst)[_stream.total_out];
        _stream.avail_out = size - _stream.total_out;

        ret = deflate(&_stream, Z_FINISH);
        if (ret == Z_STREAM_END || ret == Z_MEM_ERROR)
            break; 

        if (ret == Z_BUF_ERROR || !_stream.avail_out)
        {
#ifndef _NDEBUG
            size <<= 1;
#else
            size <<= 2;
#endif
            pointer tmp = *dst;
            *dst = zipalloc(size);
            if (!*dst)
            {
                *dst = tmp;
                goto _error;
            }
            MEMCPY(*dst, tmp, _stream.total_out);
            zipfree(tmp);
        } else if (ret == Z_ERRNO
                || ret == Z_STREAM_ERROR
                || ret == Z_DATA_ERROR
                || ret == Z_VERSION_ERROR)
        {
            goto _error;
        }
    }

    deflateEnd(&_stream);
    return _stream.total_out;

_error:
    deflateEnd(&_stream);
    zipfree(*dst); *dst = 0;
    return 0;
}

Zip::size_type Zip::unzip(pointer* dst, const_pointer src, size_type srclen)
{

    if (!dst || !src || !srclen)
        return 0;

    _stream.zalloc = 0;
    _stream.zfree = 0;
    _stream.opaque = 0;

    if (inflateInit(&_stream) != Z_OK)
        return 0;

#ifndef _NDEBUG
    size_type size = srclen << 1;
#else
    size_type size = srclen << 2;
#endif
    *dst = zipalloc(size);
    if (!*dst)
    {
        inflateEnd(&_stream);
        return 0;
    }

    _stream.next_in = const_cast<pointer>(src);
    _stream.avail_in = srclen;

    int ret = Z_OK;
    while (_stream.avail_in)
    {
        _stream.next_out = &(*dst)[_stream.total_out];
        _stream.avail_out = size - _stream.total_out;

        ret = inflate(&_stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END || ret == Z_MEM_ERROR)
            break; 

        if (ret == Z_BUF_ERROR || !_stream.avail_out)
        {
#ifndef _NDEBUG
            size <<= 1;
#else
            size <<= 2;
#endif
            pointer tmp = *dst;
            *dst = zipalloc(size);
            if (!*dst)
            {
                *dst = tmp;
                goto _error;
            }
            MEMCPY(*dst, tmp, _stream.total_out*sizeof(data_type));
            zipfree(tmp);
        }
        else if (ret == Z_NEED_DICT)
        {
            if (_dicton)
            {
                if (_dict.size() && inflateSetDictionary(&_stream,
                            &_dict.front(), _dict.size()) != Z_OK)
                    goto _error; // return 0;
                if (_stream.adler != _adler)
                    goto _error; // return 0;
            }
        }
        else if (ret == Z_ERRNO
                || ret == Z_STREAM_ERROR
                || ret == Z_DATA_ERROR
                || ret == Z_VERSION_ERROR)
        {
            goto _error;
        }
    }

    inflateEnd(&_stream);
    return _stream.total_out;

_error:
    inflateEnd(&_stream);
    zipfree(*dst); *dst = 0;
    return 0;
}

Zip::pointer Zip::zipalloc(size_type size)
{
    if (!size)
        return 0;
    size_type nsize = sizeof(data_type) * size + sizeof(zipmagic_t);
    char* tmp = BUFNEW char[nsize];
    if (tmp)
    {
        *reinterpret_cast<zipmagic_t*>(tmp) = ZIP_MAGIC;
        return reinterpret_cast<pointer>(tmp + sizeof(zipmagic_t));
    }
    return 0;
}

void Zip::zipfree(pointer ptr)
{
    if (!ptr)
        return;
    pointer tmp = reinterpret_cast<pointer>(
            reinterpret_cast<char*>(ptr) - sizeof(zipmagic_t));
    if (*reinterpret_cast<zipmagic_t*>(tmp) == ZIP_MAGIC)
        DELETEV(tmp);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

