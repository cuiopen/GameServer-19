
#include "GMSystem.h"
#include "core/Utils.h"

namespace buf
{

void GMParse(GMHandler handlers[], int nhandler, const char* cmd, int mode)
{
    if (!cmd)
        return;
    if (strlen(cmd) > 1024)
        return;

    Tokenizer tk(cmd, ' ');
    if (!tk.size())
        return;

    trim(tk[0], ' ');
    const char* pcmd = tk[0].c_str();
    int cmdlen = strlen(pcmd);
    if (!cmdlen)
        return;

    tk.pop_front();
    for (int i = 0; i < nhandler; ++i)
    {
        if (!handlers[i].name)
            continue;

        if (strncasecmp(handlers[i].name, pcmd, cmdlen) != 0)
            continue;

        if (!(handlers[i].mode & mode))
            continue;

        // XXX: same command may do different things
        handlers[i].handler(tk);
    }
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

