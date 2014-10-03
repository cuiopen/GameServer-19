
#include "CommonClient.h"
#include "core/Logger.h"
#include "core/System.h"
#include "Packet.h"

namespace buf
{

void CommonClient::onFinal()
{
    sLog.error("%s: \"I'm so tired. I must be down by calling killself().\"\n", __PRETTY_FUNCTION__);
    System::killself();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

