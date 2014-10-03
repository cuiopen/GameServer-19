
#ifndef PACKAGES_H_
#define PACKAGES_H_

#include "gamecore/Package.h"

namespace buf
{

enum pkgtype
{
    PKGTYPE_NONE = 0,
    PKGTYPE_BODY,
    PKGTYPE_NORMAL,
    PKGTYPE_STORY,
    PKGTYPE_MAX
};

// typedef u8_t pkgtype_t; /* in PackageItem.h  */

template <typename T>
class Packages : public Noncopyable
{
public:
    Packages()
    {
        MEMSET(&_pkgs, 0x00, sizeof(_pkgs));
    }

    virtual ~Packages()
    {
        for (int i = 0; i < PKGTYPE_MAX; ++i)
            DELETENULL(_pkgs[i]);
    }

    //virtual bool init() = 0;
    virtual bool init() { return false; }

private:
    Package<T>* _pkgs[PKGTYPE_MAX];
};

} // namespace buf

#endif // PACKAGES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

