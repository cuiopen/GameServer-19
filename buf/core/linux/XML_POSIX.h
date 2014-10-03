
#ifndef XML_POSIX_H_
#define XML_POSIX_H_

#include "../Utils.h"
#include "../Encoding.h"
#include "../TypeTraits.h"

#include <libxml/parser.h>
#include <string>

#define XMLPARSER_DEFOPT (XML_PARSE_NOBLANKS)

namespace buf
{

class XMLImpl
{
public:
    typedef xmlDocPtr doc_pointer;
    typedef xmlNodePtr node_pointer;
    typedef xmlAttrPtr attr_pointer;
    typedef xmlChar char_type;
    typedef xmlChar* string;
    typedef const xmlChar* const_string;
    typedef std::size_t size_type;
    typedef Encoding encoding;

public:
    XMLImpl() : _doc(0), _encoding() {}
    ~XMLImpl() { clearImpl(); }

    bool newDocImpl(const_string version);
    void setRootImpl(node_pointer root);

    node_pointer newNodeImpl(const_string name);
    void freeNodeImpl(node_pointer cur);
    node_pointer addChildImpl(node_pointer parent, node_pointer cur);
    node_pointer newChildImpl(node_pointer parent, const_string name, const_string content);
    attr_pointer newPropImpl(node_pointer cur, const_string name, const_string value);

    bool saveasImpl(const char* path, const char* encoding = "utf-8");

    bool openImpl(const char* url, const char* encoding = 0, int options = XMLPARSER_DEFOPT);
    bool readMemoryImpl(const char* buffer, size_type size, const char* encoding = 0, int options = XMLPARSER_DEFOPT);

    bool emptyImpl()const;
    const char* encodingImpl() const
    {
        return _doc?reinterpret_cast<const char*>(_doc->encoding):0;
    }

    node_pointer getRootImpl() const;

    size_type getChildNumImpl(node_pointer cur) const;
    node_pointer nextNodeImpl(node_pointer cur) const;
    node_pointer prevNodeImpl(node_pointer cur) const;
    node_pointer getFirstChildImpl(node_pointer cur) const;
    node_pointer getLastChildImpl(node_pointer cur) const;

    bool getPropertyStringImpl(node_pointer cur, const_string name, char* buffer, size_type size);
    template <typename T>
    bool getPropertyNumImpl(node_pointer cur, const_string name, T* buffer)
    {
        if (!cur || !buffer)
            return false;
        string value = xmlGetProp(cur, name);
        return  str2num(reinterpret_cast<const char*>(value), buffer);
    }

    bool getContentStringImpl(node_pointer cur, char* buffer, size_type size);
    template <typename T>
    bool getContentNumImpl(node_pointer cur, T* buffer)
    {
        if (!cur || !buffer)
            return false;
        string value = xmlNodeGetContent(cur);
        return  str2num(reinterpret_cast<const char*>(value), buffer);
    }

    void clearImpl()
    {
        if (_doc)
        {
            xmlFreeDoc(_doc);
            xmlCleanupParser();
            xmlMemoryDump();
            _doc = 0;
        }
    }

protected:
    bool value2string(const char* value, char* buffer, size_type size);

protected:
    doc_pointer _doc;
    encoding _encoding;
};

} // namespace buf

#endif // XML_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

