
#include "Cfg.h"
#include "core/Utils.h"
#include "core/Tokenizer.h"
#include "core/Socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace buf
{

// global configuration
Cfg gCfg;

Cfg::Cfg(const char* name, char* filename)
    : _loaded(false), _cfg(name?name:"Cfg")
{
    load(filename);
}

bool Cfg::load(const char* filename, bool force)
{
    if (filename && strlen(filename)) // XXX: one file will be loaded once
    {
        if (!force && strcmp(filename, _filename) == 0) // XXX: strcmp
            return false;
        else
            cpystr(filename, _filename, sizeof(_filename));
        if (!_cfg.doFile(_filename))
            return false;
        _loaded = true;
        return true;
    }
    return false;
}

bool Cfg::reload()
{
    if (strlen(_filename))
        return load(_filename, true);
    return false;
}

const char* Cfg::getCfgFile()
{
    const char* cfg = getenv("BUF_CONFIG");
    if (cfg)
        return cfg;
    return "config/config.lua";
}

Cfg::IPMask Cfg::parseAddress(const std::string& addr)
{
    Tokenizer tk(addr, '/');
    Cfg::IPMask mask = {0, 0};
    if (tk.size() == 0)
        return mask;
    if (tk.size() > 1)
        mask.mask = 32 - atoi(tk[1].c_str());
    mask.addr = Socket::toHostLong(Socket::addr2net(tk[0].c_str()));
    return mask;
}

void Cfg::setAllowedIP(const char* str)
{
    Tokenizer tk(str, ' ');
    for (size_t i = 0; i < tk.size(); ++ i)
    {
        Cfg::IPMask ipm = parseAddress(tk[i]);
        if(ipm.mask < 32)
            _IPAllowed.push_back(ipm);
    }
}

bool Cfg::isIPAllowed(const char* ip)
{
    return isIPAllowed(Socket::toHostLong(Socket::addr2net(ip)));
}

bool Cfg::isIPAllowed(u32_t ip)
{
    if(_IPAllowed.empty())
        return true;

    for(size_t i = 0; i < _IPAllowed.size(); ++i)
    {
        if(!((_IPAllowed[i].addr ^ ip) >> _IPAllowed[i].mask))
        {
#ifndef _NDEBUG
            fprintf(stderr, "ADMIN IP ALLOWED: (%u): %s\n", ip, Socket::net2addr(Socket::toNetLong(ip)));
#endif
            return true;
        }
    }
#ifndef _NDEBUG
    fprintf(stderr, "ADMIN IP IS NOT ALLOWED: (%u): %s\n", ip, Socket::net2addr(Socket::toNetLong(ip)));
#endif
    return false;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

