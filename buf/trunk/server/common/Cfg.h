
#ifndef CFG_H_
#define CFG_H_

#include "script/Script.h"
#include "core/Noncopyable.h"
#include <vector>

namespace buf
{

class Cfg : public Noncopyable 
{
public:
    static const char* getCfgFile();

public:
    Cfg(const char* name = "Cfg", char* filename = NULL);
    ~Cfg() {}

    bool load(const char* filename, bool force = false);
    bool reload();

    const char* getFile() const { return _filename; }

    template<typename T>
    inline void set(const char* name, T value)
    {
        _cfg.set(name, value);
    }

    template<typename T>
    inline T get(const char* name)
    {
        return _cfg.get<T>(name);
    }

    // XXX: xxx.xxx.xxx.xxx/24 xxx.xxx.xxx.xxx/16 xxx.xxx.xxx.xxx
    void setAllowedIP(const char* str);
    bool isIPAllowed(u32_t ip);
    bool isIPAllowed(const char* ip);

private:
    struct IPMask
    {   
        u32_t addr;
        u32_t mask;
    };
    IPMask parseAddress(const std::string& addr);

private:
    bool _loaded;
    Script _cfg;
    char _filename[PATH_MAX+1];

    std::vector<IPMask> _IPAllowed;
};

extern Cfg gCfg;

} // namespace buf

#endif // CFG_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

