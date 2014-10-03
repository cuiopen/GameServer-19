
#include "core/Utils.h"
#include "Script.h"

namespace buf
{

Script::Script(const char* name) : _L(0)
{
    cpystr(name, _name, sizeof(_name), "script");
    _L = lua_open();
    if (_L)
    {
        luaL_openlibs(_L);
        lua_tinker::init(_L);
    }
    else
    {
        fprintf(stderr, "cannot create state: not enough memory.\n");
    }
}

Script::~Script()
{
    if (_L)
        lua_close(_L);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

