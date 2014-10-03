
#include "XML_POSIX.h"
#include <string.h>

namespace buf
{

bool XMLImpl::openImpl(const char* path, const char* encoding, int options)
{
    clearImpl();
    _doc = xmlReadFile(path, encoding, options);
    if (_doc)
        _encoding.open("UTF-8", encoding);
    return _doc != 0;
}

bool XMLImpl::readMemoryImpl(const char* buffer, size_type size, const char* encoding, int options)
{
    clearImpl();
    _doc = xmlReadMemory(buffer, size, 0, encoding, options);
    return _doc != 0;
}

XMLImpl::node_pointer XMLImpl::getRootImpl() const
{
    if (!_doc)
        return 0;
    return xmlDocGetRootElement(_doc);
}

XMLImpl::size_type XMLImpl::getChildNumImpl(node_pointer cur) const
{
    if (!cur)
        return 0;
    return xmlChildElementCount(cur);
}

XMLImpl::node_pointer XMLImpl::nextNodeImpl(node_pointer cur) const
{
    if (!cur)
        return 0;
    return xmlNextElementSibling(cur);
}

XMLImpl::node_pointer XMLImpl::prevNodeImpl(node_pointer cur) const
{
    if (!cur)
        return 0;
    return xmlPreviousElementSibling(cur);
}

XMLImpl::node_pointer XMLImpl::getFirstChildImpl(node_pointer cur) const
{
    if (!cur)
        return 0;
    return xmlFirstElementChild(cur);
}

XMLImpl::node_pointer XMLImpl::getLastChildImpl(node_pointer cur) const
{
    if (!cur)
        return 0;
    return xmlLastElementChild(cur);
}

bool XMLImpl::value2string(const char* value, char* buffer, size_type size)
{
    if (!value)
        return false;
    size_type len = ::strlen(value);
    if (_encoding.encoding(value, len))
    {
        MEMCPY(buffer, &_encoding.data()[0], _encoding.size());
        len = _encoding.size();
    }
    else
    {
        MEMCPY(buffer, value, len>size-1?size-1:len);
    }
    len = len>size-1?size-1:len;
    buffer[len] = '\0';
    return true;
}

bool XMLImpl::getPropertyStringImpl(node_pointer cur, const_string name, char* buffer, size_type size)
{
    if (!cur || !name || !buffer || !size)
        return false;
    string value = xmlGetProp(cur, name);
    return value2string(reinterpret_cast<const char*>(value), buffer, size);
}

bool XMLImpl::getContentStringImpl(node_pointer cur, char* buffer, size_type size)
{
    if (!cur || !buffer || !size)
        return false;
    string value = xmlNodeGetContent(cur);
    return value2string(reinterpret_cast<const char*>(value), buffer, size);
}

bool XMLImpl::newDocImpl(const_string version)
{
    clearImpl();
    _doc = xmlNewDoc(version);
    return _doc?true:false;
}

void XMLImpl::setRootImpl(node_pointer root)
{
    if (_doc)
        xmlDocSetRootElement(_doc, root);
    return;
}

XMLImpl::node_pointer XMLImpl::newNodeImpl(const_string name)
{
    if (name && _doc)
        return xmlNewNode(0, name);
    return 0;
}

void XMLImpl::freeNodeImpl(node_pointer cur)
{
    if (cur && _doc)
        xmlFreeNode(cur);
    return;
}

XMLImpl::node_pointer XMLImpl::addChildImpl(node_pointer parent, node_pointer cur)
{
    if (parent && cur)
        return xmlAddChild(parent, cur);
    return 0;
}

XMLImpl::node_pointer XMLImpl::newChildImpl(node_pointer parent, const_string name, const_string content)
{
    if (parent && name)
        return xmlNewChild(parent, 0, name, content);
    return 0;
}

XMLImpl::attr_pointer XMLImpl::newPropImpl(node_pointer cur, const_string name, const_string value)
{
    if (cur && name)
        return xmlNewProp(cur, name, value);
    return 0;
}

bool XMLImpl::saveasImpl(const char* path, const char* encoding)
{
    if (path && _doc)
    {
        if (!encoding)
            return xmlSaveFormatFile(path, _doc, 1) == 0;
        else
            return xmlSaveFormatFileEnc(path, _doc, encoding, 1) == 0;
    }
    return false;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

