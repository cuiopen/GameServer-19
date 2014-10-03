
#include "Config.h"
#include "Url.h"
#include "Spliter.h"
#include "Utils.h"

namespace buf
{

Url::Url(const char* url)
    : _protype(URL_UNKNOW)
{
    init(url, true, true);
}

Url::Url(const std::string& url)
    : _url(url), _protype(URL_UNKNOW)
{
    init(url.c_str(), false, true);
}

Url::Url(const Url& url)
    : _url(url._url), _protype(URL_UNKNOW)
{
    init(url._url.c_str(), false, false);
}

void Url::init(const char* url, bool initurl, bool init)
{
    if (!url)
        return;

    if (initurl)
        _url = url;

    if (!_url.length())
        return;

    if (!init)
        return;

    switch (getProtocol())
    {
        case URL_HTTP:
        case URL_HTTPS:
            {
            }
            break;

        case URL_FTP:
            break;

        case URL_SYNC:
            break;

        case URL_SSH:
            break;

        case URL_MYSQL:
                initMysql();
            break;

        case URL_UNKNOW:
            break;
    }
}

urlprotocol_e Url::getProtocol()
{
    _protype = URL_UNKNOW;

    const char* pros = _url.c_str();
    const char* proe = strchr(pros, ':');

    short len = proe - pros;
    if (pros && proe && len < URL_PROTOCOL_MAX)
    {
        char buf[URL_PROTOCOL_MAX] = {0};
        buf::trim(buf);
        MEMCPY(buf, pros, len);
        buf::lower(buf);

        if (strncmp(buf, URL_MYSQL_STR, 5) == 0)
        {
            _protype = URL_MYSQL;
            _protocol = URL_MYSQL_STR;
        }
        else if (strncmp(buf, URL_HTTP_STR, 4))
        {
            _protype = URL_HTTP;
            _protocol = URL_HTTP_STR;
        }
        else if (strncmp(buf, URL_HTTPS_STR, 5))
        {
            _protype = URL_HTTPS;
            _protocol = URL_HTTPS_STR;
        }
        else if (strncmp(buf, URL_FTP_STR, 3))
        {
            _protype = URL_FTP;
            _protocol = URL_FTP_STR;
        }
        else if (strncmp(buf, URL_SSH_STR, 3))
        {
            _protype = URL_SSH;
            _protocol = URL_SSH_STR;
        }
        else if (strncmp(buf, URL_SYNC_STR, 4))
        {
            _protype = URL_SYNC;
            _protocol = URL_SYNC_STR;
        }
    }

    return _protype;
}

// mysql://user:password@host:port/db?param1&param2&...
void Url::initMysql()
{
    if (_protype != URL_MYSQL)
        return;

    size_t len = _url.length();

    const char* purl = _url.c_str();
    const char* ps = purl;
    const char* pos = ::strchr(ps, ':');
    if (pos && pos+3 < purl+len
            && *(pos+1) == '/'
            && *(pos+2) == '/')
    { // user & password
        ps = pos+3; // pointer to user
        pos = ::strchr(ps, '@');
        if (pos)
        {
            std::vector<std::string> ret;
            std::string unp(ps, pos-ps);
            buf::split(ret, unp, ':');
            if (ret.size() == 2)
            {
                _user = ret[0];
                _passwd = ret[1];
            }
            else
                return;

            if (pos+1 < purl+len)
            { // host & port
                ps = pos+1;
                pos = ::strchr(ps, '/');
                if (pos)
                {
                    ret.clear();
                    std::string hnp(ps, pos-ps);
                    buf::split(ret, hnp, ':');
                    if (ret.size())
                    {
                        _host = ret[0];
                        if (ret.size() > 1)
                        {
                            _port = ::atoi(ret[1].c_str());
                        }
                        else
                        {
                            _port = URL_MYSQL_PORT;
                        }
                    }
                    else
                        return;

                    if (pos+1 < purl+len)
                    { // db
                        ps = pos+1;
                        pos = ::strchr(ps, '?');
                        if (pos)
                        {
                            _db = std::string(ps, pos-ps);
                            if (pos+1 < purl+len)
                            { // params
                                ps = pos+1;
                                std::string params(ps, purl+len-ps);
                                buf::split(_params, params, '&');
                            }
                        }
                        else
                        {
                            _db = std::string(ps, purl+len-ps);
                        }
                        // TODO: db name check
                    }
                }
            }
        }
    }
    return;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

