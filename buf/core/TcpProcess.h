
#ifndef TCPPROCESS_H_
#define TCPPROCESS_H_

#include "Event.h"
#include "RTime.h"

namespace buf
{

enum TcpProcessState
{
    TPS_ACK = 0,
    TPS_SYNC,
    TPS_PROCESSING,
    TPS_RECYCLE,
    TPS_MAX
};

static const char* TCP_STATE_STR[] =
{
    "ack",
    "sync",
    "processing",
    "recycle",
    0
};

class TcpProcessPool; // the owner of process

class TcpProcess
{
public:
    TcpProcess(unsigned int id = 0)
        : _id(id), _cached(true), _switching(false), _created(), _state(TPS_ACK), _pool(0) {}
    virtual ~TcpProcess() {}

    virtual int ack(const void*, int) = 0;
    virtual int process(const void*, int) = 0;
    virtual int sync() = 0;
    virtual int recycle() = 0;

    virtual void heartbeat() = 0;
    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;
    virtual void onEnterProcess() = 0;

    virtual socket_t fd() const { return -1; }
    virtual const Socket* sock() const { return 0; }

    inline unsigned int id() const { return _id?_id:fd(); }
    inline unsigned int getId() const { return _id?_id:fd(); }
    inline void setId(unsigned int id) { _id = id; }

    inline void fixCreated() { _created.now(); }
    inline bool checkAckTimeout(const RTime& rt, unsigned int usecs = 10000)
    {
        fprintf(stderr, "elapse: %lu\n", _created.elapse(rt));
        return _created.elapse(rt) > usecs;
    }

    virtual bool uniqueAdd() { return true; }
    virtual void uniqueRemove() {}

    void nextState()
    {
        switch (_state)
        {
            case TPS_ACK:
                _state = TPS_SYNC;
                break;

            case TPS_SYNC:
                _state = TPS_PROCESSING;
                break;

            case TPS_PROCESSING:
                _state = TPS_RECYCLE;
                break;

            default:
                break;
        }

        if (_state >= TPS_MAX)
            _state = TPS_RECYCLE;
    }

    static const char* stateString(int state) {return state < TPS_MAX ? TCP_STATE_STR[state] : ""; }
    inline const char* toString() const { return stateString(_state);}

    inline TcpProcessState getState() const { return _state; }
    inline void setState(TcpProcessState state)
    {
        _state = state;
        if (_state > TPS_MAX)
            _state = TPS_RECYCLE;
    }

    inline bool isAck() const { return _state == TPS_ACK; }
    inline bool isSync() const { return _state == TPS_SYNC; }
    inline bool isProcessing() const { return _state == TPS_PROCESSING; }
    inline bool isRecycle() const { return _state == TPS_RECYCLE; }

    inline void setPool(const TcpProcessPool* pool) { _pool = const_cast<TcpProcessPool*>(pool); }
    inline TcpProcessPool* getPool() const { return _pool; }

    // XXX: for cache
    inline bool isCached() const { return _cached; }
    inline void setCached(bool cached)
    {
        if (_cached && !cached)
            syncs();

        _cached = cached;
    }

    // XXX: 将缓存里的数据全部发送出去，基于LT(epoll),EV_ADD(kqueue)的OUT事件
    inline int syncs()
    {
        if (sock())
            return const_cast<Socket*>(sock())->syncs();
        return 0;
    }

    inline void setSwiching(bool v = true) const { _switching = v; }
    inline bool isSwitching() const { return _switching; }

protected:
    unsigned int _id;
    bool _cached;
    volatile mutable bool _switching; // XXX: changeState ing
    RTime _created;
    TcpProcessState _state;
    TcpProcessPool* _pool;
};

class TcpProcessEvent : public Event
{
public:
    TcpProcessEvent(socket_t s, event_t events, const TcpProcess* tp)
        : Event(s, events), _tp(const_cast<TcpProcess*>(tp)) {}
    virtual ~TcpProcessEvent() { _tp = 0; }

    inline TcpProcess* tp() const { return _tp; }

protected:
    TcpProcess* _tp;
};

} // namespace bu

#endif // TCPPROCESS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

