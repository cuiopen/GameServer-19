
#ifndef URL_H_
#define URL_H_

#include <string>
#include <vector>
#include <stdlib.h> // for atoi

namespace buf
{

typedef enum
{
    URL_HTTP = 0,
    URL_HTTPS,
    URL_FTP,
    URL_SSH,
    URL_SYNC,
    URL_MYSQL,
    URL_UNKNOW
} urlprotocol_e;

const int URL_PROTOCOL_MAX = 32;
const short URL_MYSQL_PORT = 3306;

const char* const URL_HTTP_STR = "http";
const char* const URL_HTTPS_STR = "https";
const char* const URL_FTP_STR = "ftp";
const char* const URL_SSH_STR = "ssh";
const char* const URL_SYNC_STR = "sync";
const char* const URL_MYSQL_STR = "mysql";
const char* const URL_UNKNOW_STR = "unknow";

class Url
{
public:
    Url(const char* url);
    Url(const std::string& url);
    Url(const Url& url);

    ~Url(){}

    const char* protocol() const { return _protocol.c_str(); }
    const char* host() const { return _host.c_str(); }
    unsigned short port() const { return _port; }

    // ftp, mysql
    const char* user() const { return _user.c_str(); }
    const char* passwd() const { return _passwd.c_str(); }
    const char* db() const { return _db.c_str(); }

    const std::vector<std::string>& parameters() const { return _params; }

private:
    void init(const char* url, bool initurl = true, bool init = true);
    urlprotocol_e getProtocol();

    void initMysql();
    //void initHttp();
    //void initFtp();

private:
    std::string _url;

    std::string _protocol;
    urlprotocol_e _protype;

    std::string _host;
    unsigned short _port;

    std::string _user;
    std::string _passwd;
    std::string _db;

    std::vector<std::string> _params;
};

} // namespace buf

#endif // URL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

