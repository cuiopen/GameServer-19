
#include "Config.h"
#include "System.h"
#include "Logger.h"
#include "Tokenizer.h"
#ifndef NO_CPP_DEMANGLE
#include <cxxabi.h>
#endif

#ifdef UNIXLIKE

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h> // for backtrace, backtrace_symbols
#include <cxxabi.h> // for abi

namespace buf
{

void System::kill()
{
}

void System::killself()
{
    ::kill(::getpid(), SIGINT);
}

void System::callstack()
{
#define STACK_MAX 100
    void* cs[STACK_MAX] = {0};

    int i, frames = backtrace(cs, STACK_MAX);
    char** strs = backtrace_symbols(cs, frames);

#ifdef __APPLE__
    for (i = 0; i < frames; ++i)
    {
#ifndef NO_CPP_DEMANGLE
        char* yastrs = new char[strlen(strs[i])+1];
        if (yastrs)
        {
            strcpy(yastrs, strs[i]);
            int status = -1;
            char* addr = strstr(yastrs, "0x");
            *(addr-1) = '\0';
            char* fn = strchr(addr, ' ');
            *fn++ = '\0';
            char* fnend = strchr(fn, ' ');
            *fnend++ = '\0';

            char* tmp = abi::__cxa_demangle(fn, NULL, 0, &status);
            if(status == 0 && tmp)
                sLog.trace("[stack]: %s %s %s\n", yastrs, tmp, fnend);
            else
                sLog.trace("[stack]: %s\n", strs[i]);

            delete [] yastrs;
        }
        else
#endif
            sLog.trace("[stack]: %s\n", strs[i]);
    }
#else
    for (i = 0; i < frames; ++i)
    {
        sLog.trace("[stack]: %s\n", strs[i]);
    }
#endif

    free(strs);
}

#elif defined(WINDOWS)

#endif

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

