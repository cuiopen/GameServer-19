
#ifndef SYSTEM_H_
#define SYSTEM_H_

// XXX: will be implemented in OS/System.cpp

namespace buf
{

class System
{
public:
    static void kill();
    static void killself();
    static void callstack();
};

} // namespace buf

#endif // SYSTEM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

