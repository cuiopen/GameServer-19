
#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "Config.h"
#include "Noncopyable.h"
#include "Mutex.h"

namespace buf
{

template <typename T>
class Singleton : public Noncopyable
{
public:
    Singleton() {}
    ~Singleton() {}

    static void delInstance()
    {
        DELETENULL(_instance);
    }

    static T& getInstance()
    {
        if (!_instance)
        {
            ScopeLock lk(_lck);
            if (!_instance)
                _instance = BUFNEW T(); // FIXME: if failed
        }
        return *_instance;
    }

protected:
    static T* _instance;
    static Mutex _lck;
};

//template <typename T> T* Singleton<T>::_instance = BUFNEW T();
template <typename T> T* Singleton<T>::_instance = 0;
template <typename T> Mutex Singleton<T>::_lck;

} // namespace buf

#endif // SINGLETON_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

