
#ifndef XML_H_
#define XML_H_

#include "Config.h"
#ifdef UNIXLIKE 
#include "linux/XML_POSIX.h"
#endif

namespace buf
{

class XML : public XMLImpl
{
public:
    inline bool open(const char* url, const char* encoding = 0, int options = XMLPARSER_DEFOPT)
    {
        return openImpl(url, encoding, options);
    }
    inline bool readMemory(const char* buffer, size_type size, const char* encoding = 0, int options = XMLPARSER_DEFOPT)
    {
        return readMemoryImpl(buffer, size, encoding, options);
    }

    inline bool newDoc(const char* version) { return newDocImpl((const_string)version); }
    inline void setRoot(node_pointer root) { setRootImpl(root); }
    inline node_pointer getRoot() const { return getRootImpl(); }

    inline node_pointer newNode(const char* name) { return newNodeImpl((const_string)name); }
    inline void freeNode(node_pointer cur) { freeNodeImpl(cur); }

    inline node_pointer addChild(node_pointer parent, node_pointer cur) { return addChildImpl(parent, cur); }
    inline node_pointer newChild(node_pointer parent, const char* name, const char* content)
    {
        return newChildImpl(parent, (const_string)name, (const_string)content);
    }

    inline attr_pointer newProp(node_pointer cur, const char* name, const char* value)
    {
        return newPropImpl(cur, (const_string)name, (const_string)value);
    }

    inline size_type getChildNum(node_pointer cur) const { return getChildNumImpl(cur); }

    inline node_pointer nextNode(node_pointer cur) const { return nextNodeImpl(cur); }
    inline node_pointer prevNode(node_pointer cur) const { return prevNodeImpl(cur); }

    inline node_pointer getChild(node_pointer cur) const { return getFirstChild(cur); }
    inline node_pointer getFirstChild(node_pointer cur) const { return getFirstChildImpl(cur); }
    inline node_pointer getLastChild(node_pointer cur) const { return getLastChildImpl(cur); }

    inline bool getPropertyString(node_pointer cur, const char* name, char* buffer, size_type size)
    {
        return getPropertyStringImpl(cur, (const_string)name, buffer, size);
    }
    template <typename T>
    inline bool getPropertyNum(node_pointer cur, const char* name, T* buffer)
    {
        return getPropertyNumImpl(cur, (const_string)name, buffer);
    }

    inline bool getContentString(node_pointer cur, char* buffer, size_type size)
    {
        return getContentStringImpl(cur, buffer, size);
    }
    template <typename T>
    inline bool getContentNum(node_pointer cur, T* buffer)
    {
        return getContentNumImpl(cur, buffer);
    }

    inline  bool empty()const { return emptyImpl(); }
    inline  const char* encoding() const { return encodingImpl(); }
    inline  void clear() { clearImpl(); }
    inline  bool saveas(const char* path, const char* encoding = "UTF-8")
    {
        return saveasImpl(path, encoding);
    }
};

} // namespace buf

#endif // XML_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

