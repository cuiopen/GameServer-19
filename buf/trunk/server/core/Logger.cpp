
#include "Logger.h"
#include "RTime.h"
#include "Path.h"

#ifdef __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#else

#endif
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef MAX_PATH
#define MAX_PATH 4096
#endif

#define LOGGER_WHITEnBACK   ""
#define LOGGER_GREENnBACK   "\033[1;32;40m"
#define LOGGER_BLUEnBACK    "\033[1;34;40m"
#define LOGGER_YALLOWnBACK  "\033[1;33;40m"
#define LOGGER_REDnBACK     "\033[1;31;40m"
#define LOGGER_WHITEnRED    "\033[1;30;41m"
#define LOGGER_YALLOWnRED   "\033[1;33;41m"
#define LOGGER_BACKnRED     "\033[1;30;41m"

namespace buf
{

Logger sLog("sys");  // system logger

static const char* LOGGER_LEVELCOLOR[] =
{
    LOGGER_WHITEnBACK,
    LOGGER_GREENnBACK, 
    LOGGER_BLUEnBACK,
    LOGGER_YALLOWnBACK,
    LOGGER_REDnBACK,
    LOGGER_WHITEnRED,
    LOGGER_YALLOWnRED,
    LOGGER_BACKnRED
};

static const char* LOGGER_CE[] =
{
    "",
    "\033[0m",
    "\033[0m",
    "\033[0m",
    "\033[0m",
    "\033[0m",
    "\033[0m",
    "\033[0m"
};

#if defined(UNIXLIKE)
static const char* DEFAULTDIR = "/tmp";
#elif defined(WINDOWS)
static const char* DEFAULTDIR = "c:\\";
#else
static const char* DEFAULTDIR = "/tmp";
#endif

Logger::Logger()
    : _level(LOGGER_DEBUG),
    _size(0), _maxsize(LOGGER_FILE_SIZE),
    _index(0), _hour(0), _logged(false),
    _colored(false), _inited(false),
    _consoled(false), _lastlvl(LOGGER_TRACE), _file(0)
{
    setName("buf");
}

Logger::Logger(const char* name)
    : _level(LOGGER_DEBUG),
    _size(0), _maxsize(LOGGER_FILE_SIZE),
    _index(0), _hour(0), _logged(false),
    _colored(false), _inited(false),
    _consoled(false), _lastlvl(LOGGER_TRACE), _file(0)
{
    setName(name);
}

Logger::~Logger()
{
    if (_file)
        _file->close();
}

bool Logger::init(const char* dir)
{
    if (!dir || strlen(dir)==0)
        dir = DEFAULTDIR;

    Path p(dir);
    p.absolute();
    std::string path = p.toString();

    File logdir(path);
    if (!logdir.exists() && !logdir.createDirectory())
    {
        fprintf(stderr, "Create logger directory \"%s\" error!\n", path.c_str());
        return false;
    }

    if (!logdir.canWrite())
    {
        fprintf(stderr, "Directory \"%s\" can not write!\n", path.c_str());
        return false;
    }

    if (_file)
    {
        _file->close();
        _file = NULL;
    }

    cpystr(path.c_str(), _logdir, sizeof(_logdir));
    _hour = Time().hour();

    if (strlen(_name) == 0)
    {
        srand(time(0));
        char name[64] = {0};
        SNPRINTF(name, sizeof(name), "logger%d", rand()%10000);
        setName(name);
    }

    _inited = true;
    return true;
}

void Logger::changeToDir(const char* dir)
{
    if (!dir)
        return;
    char buf[MAX_PATH+1] = {0};
    changeToFile(getFileName(buf, sizeof(buf)));
}

const char* Logger::getFileName(char* buf, size_type size)
{
    if (!buf || !size)
        return 0;

    Time now;
    char* pbuf = buf;
    char* pend = buf + size;
    pbuf += SNPRINTF(pbuf, size, "%s/%s-%s.%02d",
            _logdir, _name, now.yyyymmdd('-'), now.hour());

    if (_index)
        SNPRINTF(pbuf, pend-pbuf, "-%d", _index++);
    else
        ++_index;

    buf[size-1] = '\0';

    File f(buf);
    if (f.exists() && f.size() >= _maxsize)
    {
        MEMZRO(buf, size);
        return getFileName(buf, size);
    }

    _hour = now.hour();
    return buf;
}

void Logger::changeToFile(const char* path)
{
    if (!path)
        return;

    if (_index >= LOGGER_FILES_PERHOUR) // XXX:
        return;

    bool relink = false;

    // Just create the origin file
    File tmp(path);
    if (!tmp.exists())
    {
        if (!tmp.createFile())
        {
            fprintf(stderr, "Create file %s error!\n", path);
            return;
        }
        relink = true;
    }

    size_t size = tmp.size();
    if (size >= _maxsize)
    {
        changeToDir(_logdir);
        return;
    }
#if 0
    else
    {
        fprintf(stderr, "File \"%s\" already ok with size: %u\n", path, size);
    }
#endif

    char buf[MAX_PATH+1] = {0};
    SNPRINTF(buf, sizeof(buf), "%s/%s.log", _logdir, _name);
    buf[MAX_PATH] = '\0';

    // Link to the file created before
    File* link = new File(buf);
    if (!link)
        return;

    if (relink)
    {
        if (link->exists() && !link->unLink())
        {
            fprintf(stderr, "Unlink \"%s\" error\n", buf);
            link->close();
            DELETENULL(link);
            return;
        }

        if (!link->linkTo(path))
        {
            fprintf(stderr, "Link \"%s\" error\n", buf);
            link->close();
            DELETENULL(link);
            return;
        }
    }

    _size = size;

    if (!link->open(FILE_RW))
    {
        fprintf(stderr, "Link \"%s\" open error\n", buf);
        link->close();
        DELETENULL(link);
        return;
    }

    cpystr(path, _filename, sizeof(_filename));
    if (_file)
    {
        _file->close();
        DELETENULL(_file);
    }
    _file = link;
}

void Logger::setLoggerDir(const char* dir)
{
    if (!dir)
        return;
    File path(dir);
    if (path.isDirectory())
    {
        cpystr(dir, _logdir, sizeof(_logdir));
        changeToDir(dir);
    }
}

void Logger::setLevel(const LoggerLevel_t& level)
{
    if (level == _level)
        return;

    if (level >= LOGGER_DEBUG && level < LOGGER_UNKNOW)
        _level = level;
}

Logger::size_type Logger::debug(const char* fmt, ...)
{
    if (_level > LOGGER_DEBUG)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_DEBUG, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::trace(const char* fmt, ...)
{
    if (_level > LOGGER_TRACE)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_TRACE, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::info(const char* fmt, ...)
{
    if (_level > LOGGER_INFO)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_INFO, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::warn(const char* fmt, ...)
{
    if (_level > LOGGER_WARN)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_WARN, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::error(const char* fmt, ...)
{
    if (_level > LOGGER_ERROR)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_ERROR, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::assert(const char* fmt, ...)
{
    if (_level > LOGGER_ASSERT)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_ASSERT, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::fatal(const char* fmt, ...)
{
    if (_level > LOGGER_FATAL)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(LOGGER_FATAL, fmt, ap);
    va_end(ap);
    return size;
}

Logger::size_type Logger::append(const char* fmt, ...)
{
    if (_level > LOGGER_FATAL)
        return 0;
    va_list ap;
    va_start(ap, fmt);
    size_type size = log(_lastlvl, fmt, ap, false);
    va_end(ap);
    return size;
}

Logger::size_type Logger::log(const LoggerLevel_t& level, const char* fmt, va_list ap, bool type)
{
    if (!_inited && !init(DEFAULTDIR))
            return 0;

    if (_level > level)
        return 0;

    if (_size >= _maxsize) // get a new file
        _logged = false;

    Time now;
    if (_hour != now.hour())
        _logged = false;

    if (!_logged)
    {
        _logged = true;
        changeToDir(_logdir);
        if (!_file)
            return 0;
    }

    char buf[LOGGER_MESSAGE_LEN+1] = {0};
    char* pbuf = &buf[0];
    char* pend = &buf[LOGGER_MESSAGE_LEN];

    if (isColored())
        pbuf += SNPRINTF(pbuf, pend-pbuf, "%s", LOGGER_LEVELCOLOR[level]);

    if (type)
        pbuf += SNPRINTF(pbuf, pend-pbuf, "[%s] [%s] [%s] ", now.hhmmss(':'), _name, level2string(level));

    pbuf += VSNPRINTF(pbuf, (size_t)(pend-pbuf), fmt, ap);

    if (isColored())
        pbuf += SNPRINTF(pbuf, pend-pbuf, "%s", LOGGER_CE[level]);

    if (pbuf >= pend)
        buf[LOGGER_MESSAGE_LEN] = '\0';
    else
        *pbuf = '\0';

    bool err = false;
    size_type size = sizeof(buf) - (pend - pbuf);
    if (_file)
    {
        int ret = _file->write(buf, size);
        if (ret > 0)
        {
            _size += ret;
        }
        else
        {
            fprintf(stderr, "%s", buf);
            err = true;
        }
    }
    else
    {
        err = true;
        fprintf(stderr, "%s", buf);
    }

    if (!err && _consoled)
        fprintf(stderr, "%s", buf);

    _lastlvl = level;
    return size;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

