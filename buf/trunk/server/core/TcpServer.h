
#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "Config.h"
#ifdef UNIXLIKE
#include "linux/TcpServer_POSIX.h"
#endif
#include "Thread.h"
#include "Socket.h"
#include "TcpProcessor.h"
#include "TcpProcessorPool.h"

namespace buf
{

class TcpServer : public TcpServerImpl, public Thread
{
public:
    static const int TCPS_QUEUE = 100; // wait queue
    static const int TCPS_BACKLOG = 1000; // wait queue
    static const int TCPS_MAX_PROCESSOR = 6000; // use configuration first
    static const timediff_t TICK = 5; // msecs

public:
    TcpServer(const char* name, int maxprocessor = TCPS_MAX_PROCESSOR);
    virtual ~TcpServer();

    bool init(const TPConfig* cfg = TPP_MAX);

    inline const char* getIp() const { return _ip.c_str(); }
    inline port_t getPort() const { return _port; }

    inline void setIp(const char* ip) { _ip = ip?ip:""; }
    inline void setPort(port_t port) { _port = port; }

    inline const std::string& getExtIp() const { return _extip; }
    inline port_t getExtPort() const { return _extport; }

    inline void setExtIp(const char* ip) { _ip = ip?ip:""; }
    inline void setExtPort(port_t port) { _extport = port; }

    inline bool bind(int backlog = TCPS_BACKLOG)
    {
        if (_bound || !backlog)
            return false;
        _bound = bindImpl(NULL, _port, backlog, _sock);
        return _bound;
    }

    inline bool bind(const char* ip, port_t port, int backlog = TCPS_BACKLOG)
    {
        if (_bound || !ip || !port || !backlog)
            return false;
        setIp(ip);
        setPort(port);
        if (!strlen(ip) || !strcmp(ip, "0.0.0.0") || !strcmp(ip, "127.0.0.1"))
            return bind(backlog);
        _bound = bindImpl(ip, port, backlog, _sock);
        return _bound;
    }

    virtual TcpProcessor* createProcessor(socket_t s, sockaddrin_t* sa)
    {
        (void)s;(void)sa;
        // TODO: return 0;
        return new TcpProcessor(s, sa);
    }

    inline int maxProcessor() const { return _maxprocessor; }

    virtual void final() { Thread::final(); }

    TcpProcessor* accept();
    virtual void run();

private:
    socket_t _sock;
    int _maxprocessor;
    bool _bound;

    std::string _ip;
    port_t _port;
    std::string _extip;
    port_t _extport;

    TcpProcessorPool _pool;
};

} // namespace buf

#endif // TCPSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

