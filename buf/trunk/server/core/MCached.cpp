
#include "Config.h"
#include "MCached.h"
#include "Thread.h"
#include "Tokenizer.h"

namespace buf
{

MCached::MCached() : _memsvr(0)
{
}

MCached::~MCached()
{
    if (_memsvr)
        memcached_free(_memsvr);
    _memsvr = 0;
}

bool MCached::init(const char* hosts)
{
    return pushHost(hosts);
}

bool MCached::init(const std::string& hosts)
{
    return init(hosts.c_str());
}

bool MCached::pushHost(const std::string& hosts)
{
    return pushHost(hosts.c_str());
}

bool MCached::pushHost(const char* hosts)
{
    if (!hosts || !strlen(hosts))
        return false;

    if (!_memsvr)
    {
        _memsvr = memcached_create(NULL);
        if (!_memsvr)
            return false;
    }

    memcached_return rc;
    memcached_server_st* servers = memcached_servers_parse(hosts);
    if (servers)
    {
        rc = memcached_server_push(_memsvr, servers);
        memcached_server_free(servers);
        if (rc == MEMCACHED_SUCCESS)
        {
            addHosts(hosts);
            return true;
        }
    }
    return false;
}

bool MCached::addHosts(const char* hosts)
{
    if (!hosts || !strlen(hosts))
        return false;
    Tokenizer tk(hosts, ',');
    if (!tk.size())
        return false;

    bool ret = false;
    Tokenizer::size_type sz = tk.size();
    for (Tokenizer::size_type i = 0; i < sz; ++i)
    {
        bool found = false;
        for (unsigned int n = 0; n < _hosts.size(); ++n)
        {
            if (!tk[i].empty())
                continue;
            if (tk[i] == _hosts[n])
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            _hosts.push_back(tk[i]);
            ret = true;
        }
    }
    return ret;
}

const char* MCached::get(const char* key, char* value, size_t size)
{
    if (!_memsvr || !key || !value || !size)
        return 0;
    return get(key, strlen(key), value, size);
}

const char* MCached::get(const char* key, size_t key_size, char* value, size_t size)
{
    if (!_memsvr || !key || !key_size || !value || !size)
        return 0;

    size_t tlen = 0;
    unsigned int flags = 0;

    int retry = 3;
    memcached_return rc;

    while (retry)
    {
        --retry;
        char* rval = memcached_get(_memsvr, key, key_size, &tlen, &flags, &rc);
        if (rc == MEMCACHED_SUCCESS && rval && tlen)
        {
            tlen = tlen > size ? size : tlen;
            MEMCPY(value, rval, tlen);
            free(rval);
            break;
        }
        else
        {
            Thread::usleep(500);
        }
    }
    return value;
}

bool MCached::set(const char* key, const char* value, int timeout)
{
    if (!_memsvr || !key || !value)
        return 0;
    return set(key, strlen(key), value, strlen(value), timeout);
}

bool MCached::set(const char* key, size_t key_size, const char* value, size_t size, int timeout)
{
    if (!_memsvr || !key || !key_size || !value || !size)
        return false;

    memcached_return_t rc = memcached_set(_memsvr, key, key_size, value, size, timeout, 0);

    int retry = 2;
    while (rc != MEMCACHED_SUCCESS && retry)
    {
        rc = memcached_set(_memsvr, key, key_size, value, size, timeout, 0);
        --retry;
    }

    return rc == MEMCACHED_SUCCESS;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

