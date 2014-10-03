
#ifndef VERSION_H_
#define VERSION_H_

#ifndef MAJOR_VERSION
#define MAJOR_VERSION 0
#endif

#ifndef MINOR_VERSION
#define MINOR_VERSION 0
#endif

#ifndef MICRO_VERSION
#define MICRO_VERSION 0
#endif

#define _S(x) #x

#ifndef VERSION
#define VERSION _S(MAJOR_VERSION)"."_S(MINOR_VERSION)"."_S(MICRO_VERSION)
#endif

#ifndef BUILD_VERSION
#define BUILD_VERSION "build.YYYYMMDDHHMMSS"
#endif

#endif // VERSION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

