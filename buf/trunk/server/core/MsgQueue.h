
#ifndef MSGQUEUE_H_
#define MSGQUEUE_H_

#include "Config.h"
#ifdef USE_MEM_POOL
#include "MemPool.h"
#endif
#include <queue>

/*
 * 每一个MsgQueue有且只有一个写者，有且有一个读者
 * 需要flushToArray来将队列里的数据包压力循环队列
 * 心跳包可以做到
 **/

#ifndef MSG_QUEUE_SIZE
#define MSG_QUEUE_SIZE 1024
#endif

namespace buf
{

typedef std::pair<unsigned int, unsigned char*> MsgPair;
typedef std::pair<volatile bool, MsgPair> Queue;

template <int SIZE = MSG_QUEUE_SIZE>
class MsgQueue
{
public:
    MsgQueue() : rqueue(0), wqueue(0) {}

    ~MsgQueue()
    {
        clear();
    }

    MsgPair* get()
    {
        MsgPair *ret = 0;
        if(cmdQueue[rqueue].first)
        {
            ret = &cmdQueue[rqueue].second;
        }
        return ret;
    }

    void erase()
    {
#ifdef USE_MEM_POOL
        _mp.free(cmdQueue[rqueue].second.second);
#else
        DELETEV(cmdQueue[rqueue].second.second);
#endif
        cmdQueue[rqueue].first = false;
        ++rqueue;
        rqueue = rqueue % SIZE;
    }

    unsigned char* getMsg() const
    {
        return cmdQueue[rqueue].second.second;
    }

    bool put(const void* cmd, const unsigned int len)
    {
#ifdef USE_MEM_POOL
        unsigned char* buf = reinterpret_cast<unsigned char*>(_mp.alloc(len+4));
#else
        unsigned char* buf = BUFNEW unsigned char[len+4];
#endif
        if (buf)
        {
            MEMCPY(buf, cmd, len);
            if (!flushToArray() && !cmdQueue[wqueue].first)
            {
                cmdQueue[wqueue].second.first = len;
                cmdQueue[wqueue].second.second = buf;
                cmdQueue[wqueue].first = true;
                ++wqueue;
                wqueue = wqueue % SIZE;
                return true;
            }
            else
            {
                queueMsg.push(std::make_pair(len, buf));
            }
            return true;
        }
        return false;
    }

private:
    void clear()
    {
        while (flushToArray())
        {
            while (get())
            {
                erase();
            }
        }
        while (get())
        {
            erase();
        }
    }

public:
    bool flushToArray()
    {
        bool isLeft=false;
        while (!queueMsg.empty())
        {
            if (!cmdQueue[wqueue].first)
            {
                cmdQueue[wqueue].second = queueMsg.front();
                cmdQueue[wqueue].first = true;
                ++wqueue;
                wqueue = wqueue % SIZE;
                queueMsg.pop();
            }
            else
            {
                isLeft = true;
                break;
            }
        }
        return isLeft;
    }

    void printInfo()
    {
        fprintf(stderr, "Size: %lu\n", sizeof(*this));
#ifdef USE_MEM_POOL
        _mp.printInfo();
#endif
    }

private:
    Queue cmdQueue[SIZE];
    std::queue<MsgPair, std::deque<MsgPair> > queueMsg;
    unsigned int rqueue;
    unsigned int wqueue;

#ifdef USE_MEM_POOL
    MemPool _mp;
#endif
};

class MessageQueue
{
public:
    MessageQueue() {}
    virtual ~MessageQueue() {}

    bool pushCmd(const void* cmd, unsigned int len)
    {
        return cmdQueue.put(cmd, len);
    }

    // XXX:
    virtual bool cmdProcessor(const void*, unsigned int) { return false; }

    // XXX: will be called by a time tick thread
    bool doProcess()
    {
        MsgPair* cmd = cmdQueue.get();
        while (cmd)
        {
            cmdProcessor(cmd->second, cmd->first);
            cmdQueue.erase();
            cmd = cmdQueue.get();
        }
        if (cmd)
        {
            cmdQueue.erase();
        }
        return true;
    }

    unsigned char* getMsg() const
    {
        return cmdQueue.getMsg();
    }

    void printInfo()
    {
        cmdQueue.printInfo();
    }

private:
    MsgQueue<> cmdQueue;
};


} // namespace buf

#endif // MSGQUEUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

