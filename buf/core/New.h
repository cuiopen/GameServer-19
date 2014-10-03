
#ifndef BUFNEW_H_
#define BUFNEW_H_

#ifndef _NDEBUG
#include <stdio.h> // for size_t

void* operator new(size_t size, void* ptr, const char* file, int line) throw();
void* operator new[](size_t size, void* ptr, const char* file, int line) throw();

void* operator new(size_t size, const char* file, int line) throw();
void* operator new[](size_t size, const char* file, int line) throw();

void operator delete(void* ptr, const char* file, int line) throw();
// FIXME: do not delete an object array specially when the object has destructor
void operator delete[](void* ptr, const char* file, int line) throw();
void operator delete[](void* ptr, size_t size, const char* file, int line) throw();

#define BUFNEW new(__FILE__, __LINE__)
#define PLACEBUFNEW(x) new((x), __FILE__, __LINE__)

#else // !_NDEBUG

#define BUFNEW new
#define PLACEBUFNEW(x) new((x))
#endif // _NDEBUG

#define DELETE(x) delete((x))
#define DELETEV(x) delete[](x)

#endif // BUFNEW_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

