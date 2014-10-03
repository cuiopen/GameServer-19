
#ifndef THREAD_H_
#define THREAD_H_

#include "Config.h"
#ifdef UNIXLIKE
#include "linux/Thread_POSIX.h"
#endif

#include <string>

namespace buf
{

class Thread : public ThreadImpl
{
public:
    inline static void sleep(unsigned long secs) { sleepImpl(secs); }
    inline static void msleep(unsigned long msecs) { msleepImpl(msecs); }
    inline static void usleep(unsigned long usecs) { usleepImpl(usecs); }

public:
    Thread() {}
    Thread(const char* name, bool joinable = true)
        : ThreadImpl(name, joinable) {}
    virtual ~Thread() {}

    inline const char* getName() const { return getNameImpl(); }
    inline void setName(const char* name) { setNameImpl(name); }

    inline threadid_t tid() const { return tidImpl(); }

    bool start() { return startImpl(); }
    void join() { joinImpl(); }

    inline void final() { terminate(); }
    inline void terminate() { terminateImpl(); }

    inline bool isTerminated() const { return isTerminatedImpl(); }
    inline bool isFinal() const { return isTerminated(); }
    inline bool isAlive() const { return isAliveImpl(); }
    inline bool isRunning() const { return isRunningImpl(); }
    inline bool isJoinable() const { return isJoinableImpl(); }

    virtual void run() {
        while (true) {
            sleep(1);
            printf("%s\n", getName());
        }
    }
};

} // namespace buf

#endif // THREAD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

