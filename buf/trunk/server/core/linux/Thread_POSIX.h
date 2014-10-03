
#ifndef THREAD_POSIX_H_
#define THREAD_POSIX_H_

#include <string>

#include "../Config.h"
#include "../Cond.h"
#include "../Mutex.h"
#include "../Utils.h"

namespace buf
{

typedef pthread_t threadid_t;

class ThreadImpl
{
public:
    inline static void sleepImpl(unsigned long secs) { ::sleep(secs); }
    inline static void msleepImpl(unsigned long msecs) { ::usleep(msecs*1000); }
    inline static void usleepImpl(unsigned long usecs) { ::usleep(usecs); }

public:
    ThreadImpl(bool joinable = true)
        : _joinable(joinable), _tid(0), _isrunning(false), _alive(false)
    {
        cpystr("THREAD", _name, sizeof(_name));
    }
    ThreadImpl(const char* name, bool joinable = true)
        : _joinable(joinable), _tid(0), _isrunning(false), _alive(false)
    {
        cpystr(name, _name, sizeof(_name), "THREAD");
    }
    ~ThreadImpl() { _alive = false; }

    inline const char* getNameImpl() const { return _name; }
    inline void setNameImpl(const char* name) { cpystr(name, _name, sizeof(_name), "THREAD"); }

    threadid_t tidImpl() const { return _tid; }

    bool startImpl();
    void joinImpl();

    inline void finalImpl() { terminateImpl(); }
    inline void terminateImpl() { _isrunning = false; }

    inline bool isTerminatedImpl() const { return _isrunning == false; }
    inline bool isAliveImpl() const { return _alive; }
    inline bool isRunningImpl() const { return _isrunning; }
    inline bool isJoinableImpl() const { return _joinable; }

    virtual void run() = 0;

private:
    static void* start_run(void*);

private:
    char _name[NAME_MAX+1];
    bool _joinable;
    threadid_t _tid;

    volatile bool _isrunning;
    volatile bool _alive;

    Mutex _lock;
    Cond _cond;
};

} // namespace buf

#endif // THREAD_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

