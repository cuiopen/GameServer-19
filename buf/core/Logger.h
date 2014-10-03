
#ifndef LOGGER_H_
#define LOGGER_H_

#include "Config.h"
#include "File.h"
#include "Utils.h"

#include <stdarg.h>
#include <string>

namespace buf
{

typedef enum LoggerLevel
{
    LOGGER_DEBUG = 0,
    LOGGER_TRACE,
    LOGGER_INFO,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_ASSERT,
    LOGGER_FATAL,
    LOGGER_UNKNOW
} LoggerLevel_t;

static const char* LOGGER_LEVEL2STRING[] =
{
    "DEBUG",
    "TRACE",
    "INFO",
    "WARN",
    "ERROR",
    "ASSERT",
    "FATAL",
    "UNKNOW"
};

inline static const char* level2string(const LoggerLevel_t& level)
{
    if (level >= LOGGER_DEBUG && level < LOGGER_UNKNOW)
        return LOGGER_LEVEL2STRING[level];
    return 0;
}

//#define _NDEBUG
#define LOGGER_MESSAGE_LEN 4096
#ifndef _NDEBUG
#define LOGGER_FILE_SIZE 40960
#define LOGGER_FILES_PERHOUR ((int)60)
#else
#define LOGGER_FILE_SIZE 0x7FFFFFFF
#define LOGGER_FILES_PERHOUR ((int)0x7FFFFFFF)
#endif

class Logger
{
public:
    typedef size_t size_type;

public:
    Logger();
    explicit Logger(const char* name);
    ~Logger();

    bool init(const char* dir = NULL);

    inline const char* getName() const { return _name; }
    inline void setName(const char* name)
    {
        if (name)
            cpystr(name, _name, sizeof(_name));
        else
            MEMCPY(_name, "buf", 3);
    }
    void setLoggerDir(const char* dir);
    inline const char* getLoggerDir() const { return _logdir; }
    inline const char* getLoggerPath() const { return _filename; }

    void setLevel(const LoggerLevel_t& level);
    LoggerLevel_t getLevel() const { return _level; }

    inline void setMaxSize(size_type size) { _maxsize = size; }
    inline size_type getMaxSize() const { return _maxsize; }

    inline void setColored(bool v) { _colored = v; }
    inline bool isColored() const { return _colored; }

    inline void setConsole(bool v) { _consoled = v; }
    inline bool getConsole() const { return _consoled; }

    size_type debug(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type trace(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type info(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type warn(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type error(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type assert(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type fatal(const char* fmt, ...) ATTR_PRINTF(2,3);
    size_type append(const char* fmt, ...) ATTR_PRINTF(2,3);

private:
    void changeToDir(const char* dir);
    void changeToFile(const char* path);
    const char* getFileName(char* buf, size_type size);
    size_type log(const LoggerLevel_t& level, const char* fmt, va_list ap, bool type = true);

private:
    char _name[NAME_MAX+1]; // logger name
    char _logdir[PATH_MAX+1]; // logger directory
    char _filename[PATH_MAX+1]; // current logging file
    LoggerLevel_t _level; // logger level
    size_type _size; // size of now logging file
    size_type _maxsize; // max size of a log file
    int _index; // the index file in one hour
    unsigned char _hour; // current hour
    bool _logged; // is logging
    bool _colored; // color
    bool _inited;
    bool _consoled;
    LoggerLevel_t _lastlvl;
    File* _file;
};

extern Logger sLog; // logger for core or common
extern Logger sLog; // global/game logger, init() must be called before write log

#define LOG_DEBUG(...)              \
    do {                            \
        sLog.debug(__VA_ARGS__); \
    } while (0)

#define LOG_TRACE(...)              \
    do {                            \
        sLog.trace(__VA_ARGS__); \
    } while (0)

#define LOG_INFO(...)               \
    do {                            \
        sLog.info(__VA_ARGS__);  \
    } while (0)

#define LOG_WARN(...)               \
    do {                            \
        sLog.warn(__VA_ARGS__);  \
    } while (0)

#define LOG_ERROR(...)              \
    do {                            \
        sLog.error(__VA_ARGS__); \
    } while (0)

#define LOG_FATL(...)               \
    do {                            \
        sLog.fatal(__VA_ARGS__); \
    } while (0)

} // namespace buf

#endif // LOGGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

