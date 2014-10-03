
#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef __cplusplus
#   define __BEGIN extern "C" {
#   define __END }
#else
#   define __BEGIN
#   define __END
#endif

// hardware macros
#if defined(__i386__)   \
    || defined(i386)    \
    || defined(_X86_)   \
    || defined(_M_IX86) \
    || defined(_WIN32)
#   define CPU_32BIT 1
#endif

#if defined(__x86_64__)     \
    || defined(__amd64__)   \
    || defined(__amd64)     \
    || defined(_WIN64)
#   define CPU_64BIT 1
#endif

#define SMP 1

// #define BIG_ENDIAN 0
#if defined(__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__ && !defined(LITTLE_ENDIAN)
#   define LITTLE_ENDIAN 1
#endif

#if defined(__BIG_ENDIAN__) && __BIG_ENDIAN__ && !defined(BIG_ENDIAN)
#   define BIG_ENDIAN 1
#endif

// OS
#define _OS_FREE_BSD      0x0001
#define _OS_LINUX         0x0005
#define _OS_MAC_OS_X      0x0006
#define _OS_NET_BSD       0x0007
#define _OS_OPEN_BSD      0x0008
#define _OS_CYGWIN        0x000d
#define _OS_UNKNOWN_UNIX  0x00ff
#define _OS_WINDOWS_NT    0x1001

#if defined(__FreeBSD__)
#   define _OS_FAMILY_UNIX 1
#   define _OS_FAMILY_BSD 1
#   define _OS _OS_FREE_BSD
#   define UNIXLIKE 1
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
#   define _OS_FAMILY_UNIX 1
#   define _OS _OS_LINUX
#   define UNIXLIKE 1
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#   define _OS_FAMILY_UNIX 1
#   define _OS_FAMILY_BSD 1
#   define _OS _OS_MAC_OS_X
#   define UNIXLIKE 1
#elif defined(__NetBSD__)
#   define _OS_FAMILY_UNIX 1
#   define _OS_FAMILY_BSD 1
#   define _OS _OS_NET_BSD
#   define UNIXLIKE 1
#elif defined(__OpenBSD__)
#   define _OS_FAMILY_UNIX 1
#   define _OS_FAMILY_BSD 1
#   define _OS _OS_OPEN_BSD
#   define UNIXLIKE 1
#elif defined(unix) || defined(__unix) || defined(__unix__)
#   define _OS_FAMILY_UNIX 1
#   define _OS _OS_UNKNOWN_UNIX
#   define UNIXLIKE 1
#elif defined(_WIN32) || defined(_WIN64)
#   define _OS_FAMILY_WINDOWS 1
#   define _OS _OS_WINDOWS_NT
#   define WINDOWS 1
#elif defined(__CYGWIN__)
#   define _OS_FAMILY_UNIX 1
#   define _OS _OS_CYGWIN
#   define UNIXLIKE 1
#endif

// compiler macros

#ifndef NULL
#   ifdef __cplusplus
#       define NULL 0
#   else
#       define NULL ((void*)0)
#   endif
#endif

#if defined(__GNUC__) || defined(__CYGWIN__)
#define __COMPILER_GCC 1
#   ifdef __cplusplus
#       define __COMPILER_CXX 1
#   endif
#endif

#if defined(_MSC_VER)
#   define COMPILER_MSVC 1
#   if _MSC_VER < 1400
#       define __COMPILER_MSVC7_OR_LOWER 1
#   elif _MSC_VER < 1600
#       define __COMPILER_MSVC9_OR_LOWER 1
#   endif
#endif

#define COMPILER(x) defined (__COMPILER_##x)

#if COMPILER(GCC) || COMPILER(CXX)
#   include <stddef.h>
#else
#   define __PRETTY_FUNCTION__ __func__
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 4 && __GNUC_MINOR__ >= 3))
#   define CXX0X 1
#endif

#if defined(__GNUC__)
#   include <limits.h>
#endif

#if COMPILER(GCC)
#   define ALIGNOF(type) __alignof__(type)
#   define ALIGNMENT(type, varname, alignment) type varname __attribute__((__aligned__(alignment)))
#elif COMPILER(MSVC)
#   define ALIGNOF(type) __alignof(type)
#   define ALIGNMENT(type, varname, alignment) __declspec(align(alignment)) type varname
#else
#   error need alignment
#endif

// constants
#if defined(DEFAULT_TRUNK_SIZE) && DEFAULT_TRUNK_SIZE >= (8*1024)
#   define TRUNK_SIZE DEFAULT_TRUNK_SIZE
#else
#   define TRUNK_SIZE (8*1024)
#endif

// size of send/recv buffer of socket
#if TRUNK_SIZE < (8*1024)
#   define RECV_BUF_SIZE (8*1024)
#   define SEND_BUF_SIZE (8*1024)
#else
#   define RECV_BUF_SIZE TRUNK_SIZE
#   define SEND_BUF_SIZE TRUNK_SIZE
#endif

#if !defined(SOCK_BUF_MAX_SIZE) || SOCK_BUF_MAX_SIZE < TRUNK_SIZE || SOCK_BUF_MAX_SIZE < (8*1024)
#   define SOCK_BUF_MAX_SIZE TRUNK_SIZE
#endif

// utilies
#undef MAX
#define MAX(x, y) ((x)>(y)?(x):(y))

#undef MIN
#define MIN(x, y) ((x)<(y)?(x):(y))

// swap tow numbers
#undef SWAP
#define SWAP(x, y) (x)^=(y);(y)^=(x);(x)^=(y)

#if COMPILER(GCC) || COMPILER(CXX)
#   define ATTR_NORETURN __attribute__((noreturn))
#   define ATTR_PRINTF(F,V) __attribute__ ((format (printf, F, V)))
#   define CONSTRUCTOR __attribute__((constructor))
#   define DESTRUCTOR __attribute__((destructor))
#else
#   define ATTR_NORETURN
#   define ATTR_PRINTF(F,V)
#   define CONSTRUCTOR
#   define DESTRUCTOR
#endif

// unuse
#undef UNUSE
#define UNUSE(x) ((void)(x))

#include "New.h"

#define DELETENULL(x) ((x)!=NULL?(DELETE(x)),(x)=NULL:NULL)
#define DELETEVEC(x) ((x)!=NULL?(DELETEV(x)),(x)=NULL:NULL)

#define CLRERR() (errno=0)

// memory
#ifdef UNIXLIKE
#   include <string.h>
#   define MEMZRO(d,s) memset((d), 0x00, (s))
#   define MEMSET memset
#   define MEMCPY memcpy
#   define MEMMOVE memmove
#elif defined(WINDOWS)
#   include <Windows.h>
#   define MEMZRO(d,s) ZeroMemory((d), (s))
#   define MEMSET memset
#   define MEMCPY memcpy
#   define MEMMOVE memmove
#else
#   error "system error"
#endif

// string format
#if defined(UNIXLIKE)
#   define SNPRINTF    snprintf
#   define VSNPRINTF   vsnprintf
#endif
#if defined(WINDOWS)
#   define SNPRINTF    _snprintf
#   define VSNPRINTF   _vsnprintf
#   include <io.h>
#endif

#ifdef CPU_64BIT
#   define I64_FMT "l"
#else
#   define I64_FMT "ll"
#endif
#define SZT_FMT "lu"

// debug
#ifndef _NDEBUG
#   define DEBUG(x,...) fprintf(stderr, x, ##__VA_ARGS__)
#else
#   define DEBUG(x,...)
#endif

// system
#ifndef TEMP_FAILURE_RETRY
#   if defined(__GNUC__)
#       define TEMP_FAILURE_RETRY(expr)                     \
        (                                                   \
            __extension__                                   \
            ({                                              \
                CLRERR();                                   \
                long int __result;                          \
                do __result = (long int)(expr);             \
                while(__result == -1L&& errno == EINTR);    \
                __result;                                   \
            })                                              \
        )
#   else
#       define TEMP_FAILURE_RETRY(expr) (expr)
#   endif
#endif

#include "Consts.h"

// typedefs definiation
typedef unsigned char       u8_t;
typedef unsigned short      u16_t;
typedef unsigned int        u32_t;
#ifdef CPU_64BIT
typedef unsigned long       u64_t;
#else
typedef unsigned long long  u64_t;
#endif

typedef float               f32_t;
typedef double              f64_t;
typedef long double         f128_t;

typedef char                s8_t;
typedef short               s16_t;
typedef int                 s32_t;
#ifdef CPU_64BIT
typedef long                s64_t;
#else
typedef long long           s64_t;
#endif

typedef u16_t               strlen_t;

#define VOLATILE volatile 

#endif // CONFIG_H_ 

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

