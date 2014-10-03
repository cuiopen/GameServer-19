
#ifndef THREADGROUP_H_
#define THREADGROUP_H_

#include "Config.h"
#include "Thread.h"
#include "Mutex.h"
#include "Utils.h"
#include <vector>

#ifndef MAX_THREADS
#ifndef _NDEBUG
#define MAX_THREADS 32
#else
#define MAX_THREADS 4096
#endif
#endif

namespace buf
{

class ThreadGroup
{
public:
    typedef std::vector<Thread*> container;
    typedef container::size_type size_type;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    ThreadGroup(const char* name, const size_type maxsize = MAX_THREADS) 
        : _size(0), _max(maxsize), _lock(), _container(maxsize, NULL)
    {
        cpystr(name, _name, sizeof(_name));
    }

    ~ThreadGroup()
    {
        ScopeLock lock(_lock);
        for (size_type i = 0; i < _size; ++i) {
            DELETENULL(_container[i]);
        }
    }

    bool put(const Thread* thread);
    const Thread* get(threadid_t id) const;

    inline const char* getName() const { return _name; }
    const Thread* operator[](size_type index) const;
    Thread* operator[](size_type index);

    inline iterator begin() { return _container.begin(); }
    inline iterator end() { return  _container.end(); }
    inline const_iterator begin() const { return _container.begin(); }
    inline const_iterator end() const { return _container.end(); }
    bool push_back(const Thread* thread) { return put(thread); }

    bool erase(const Thread* thread);
    bool erase(const char* threadname);
    bool erase(const threadid_t id);

    inline bool erase(const std::string& threadname) { return erase(threadname.c_str()); }

    void startAll();
    void joinAll();
    void stopAll();

    inline size_type size() const
    {
        ScopeLock lock(_lock);
        return _size;
    }

    inline size_type max_size () const { return _max; }

private:
    size_type _size;
    size_type _max;
    char _name[NAME_MAX+1];
    mutable Mutex _lock;
    container _container;
};

} // namespace buf

#endif // THREADGROUP_H_

