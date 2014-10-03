
#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include "core/Noncopyable.h"
#include "core/Buffer.h"
#include "core/File.h"
#include "common/GameObjectMgr.h"

/**
 * This is a manager which maps data from database or binary files to
 * data struction in memory
 **/

namespace buf
{

// XXX: MEM must supply MEM::init(const BIN*) and be drived from GameObject
// XXX: BIN must packed by 1
// XXX: C must supply C::find(MEM::id_type, MEM*& ret), C::push(MEM*)

template <typename MEM, typename BIN, typename C = GameObjectMgr<GOMgrById<typename MEM::id_type, MEM*>, GOMgrByMultiName<MEM*> > >
class DataManager : public C, public Noncopyable
{
public:
    typedef MEM* pointer;
    typedef const MEM* const_pointer;

public:
    DataManager(const char* file = 0) : _file(file?file:"") {}
    virtual ~DataManager()
    {
        // TODO:
    }

    virtual bool loadFromFile(const char* file = 0)
    {
        if (!file && !_file.length())
            return false;

        int blksize = sizeof(BIN);
        StackBuffer<sizeof(BIN)+32> buf;

        int ret = 0;
        int size = 0;
        File f(file);
        if (f.open(FILE_R))
        {
            while (true)
            {
                do
                {
                    ret = f.read(buf.wbuf(), blksize);
                    if (ret > 0)
                    {
                        buf.wflip(ret);
                        size += ret;
                    }
                } while (ret && ret > 0 && size < blksize);

                if (ret <= 0 || size <= 0 || size < blksize)
                {
                    // TODO: log
                    break;
                }

                MEM* m = BUFNEW MEM;
                if (!m) return false;

                m->init((BIN*)(buf.rbuf()));
                push(m);
                buf.clear();
            }
            f.close();
            return true;
        }
        return false;
    }

    // XXX: DANGROUS IF DATA REFERENCED BY OTHRE MODULE
    bool reloadFromFile(const char* file = 0)
    {
        if (!file && !_file.length())
            return false;
        //_data.destory();
        return true;
    }

    virtual bool loadFromDB(BIN* b)
    {
        MEM* m = BUFNEW MEM;
        if (!m)
            return false;
        m->init(b);
        push(m);
        return true;
    }

    inline const MEM* getById(typename MEM::id_type id) const
    {
        MEM* ret = 0;
        find(id, ret);
        return ret;
    }

private:
    std::string _file;
};

} // namespace buf

#endif // DATAMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

