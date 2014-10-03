
#ifndef PACKET_H_
#define PACKET_H_

#include "core/Buffer.h"
#include "Cmd.h"

namespace buf
{

class Packet : public Buffer
{
public:
    Packet() : Buffer(256) {}

    explicit Packet(CMD_t cmd, Buffer::size_type size = 256) : Buffer(size)
    {
        (*this) << cmd;
    }

    ~Packet() {}

    inline CMD_t getCmd() { return at<CMD_t>(); };
    inline void setCmd(CMD_t& cmd) { at<CMD_t>() = cmd; };

};

} // namespace buf

#endif // PACKET_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

