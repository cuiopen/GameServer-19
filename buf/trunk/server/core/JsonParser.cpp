
#include "JsonParser.h"
#include "File.h"

namespace buf
{

JsonParser::JsonParser() : _root(NULL), _dirty(true)
{
    root();
}

JsonParser::~JsonParser()
{
    clear();
}

void JsonParser::clear()
{
    if (_root)
    {
        json_free_value(&_root);
        _root = NULL;
    }
    if (!_strfmt.empty())
        _strfmt.clear();
}

bool JsonParser::parse(const char* ctx)
{
    if (!ctx)
        return false;

    Json* newroot = NULL;
    if (json_parse_document(&newroot, const_cast<char*>(ctx)) != JsonOk)
        return false;

    clear();
    _root = newroot;
    return true;
}

bool JsonParser::parseFile(const char* filename)
{
    if (!filename)
        return false;

    FILE* file = fopen(filename, "r");
    if (!file)
        return false;

    Json* newroot = NULL;
    if (json_stream_parse(file, &newroot) != JsonOk)
        return false;

    clear();
    _root = newroot;

    fclose(file);
    return true;
}

bool JsonParser::saveFile(const char* filename)
{
    if (!filename)
        return false;

    FILE* file = fopen(filename, "w+");
    if (!file)
        return false;

    if (json_stream_output(file, _root) != JsonOk)
    {
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

Json* JsonParser::newObject(Json* parent)
{
    Json* j = json_new_object();
    if (!j)
        return NULL;
    if (parent)
        json_insert_child(parent, j);
    return j;
}

Json* JsonParser::newTrue(const char* name, Json* parent)
{
    Json* jvalue = json_new_true();
    if (!jvalue)
        return NULL;

    if (!parent)
        parent = root();

    Json* v = newValue(name, jvalue, parent);
    if (!v)
        json_free_value(&jvalue);

    return parent;
}

Json* JsonParser::newFalse(const char* name, Json* parent)
{
    Json* jvalue = json_new_false();
    if (!jvalue)
        return NULL;

    if (!parent)
        parent = root();

    Json* v = newValue(name, jvalue, parent);
    if (!v)
        json_free_value(&jvalue);

    return parent;
}

Json* JsonParser::newNull(const char* name, Json* parent)
{
    Json* jvalue = json_new_null();
    if (!jvalue)
        return NULL;

    if (!parent)
        parent = root();

    Json* v = newValue(name, jvalue, parent);
    if (!v)
        json_free_value(&jvalue);

    return parent;
}

Json* JsonParser::newValue(const char* name, Json* value, Json* parent)
{
    if (!name || !value)
        return NULL;

    bool newparent = false;
    if (!parent)
    {
        parent = json_new_object();
        if (!parent)
            return NULL;
        newparent = true;
    }

    if (json_insert_pair_into_object(parent, name, value) != JsonOk)
    {
        if (newparent)
            json_free_value(&parent);
        return NULL;
    }

    _dirty = true;
    return parent;
}

Json* JsonParser::newString(const char* name, const char* value, Json* parent)
{
    if (!name || !value)
        return NULL;

    Json* jvalue = json_new_string(value);
    if (!jvalue)
        return NULL;

    Json* v = newValue(name, jvalue, parent);
    if (!v)
        json_free_value(&jvalue);

    return v;
}

bool JsonParser::addChild(Json* child, Json* parent)
{
    if (!child || !parent)
        return NULL;

    _dirty = true;
    return json_insert_child(parent, child) == JsonOk;
}

Json* JsonParser::newArray(const char* name, Json* parent)
{
    Json* arr = json_new_array();

    if (!parent)
        parent = root();

    if (!parent)
    {
        parent = newObject();
    }

    if (arr && parent)
    {
        if (json_insert_pair_into_object(parent, name, arr) != JsonOk)
        {
            json_free_value(&arr);
            return NULL;
        }
    }

    _dirty = true;
    return arr;
}

Json* JsonParser::find(const char* name, Json* root)
{
    if (!name)
        return NULL;

    if (root)
        return json_find_first_label(root, name);

    return json_find_first_label(this->root(), name);
}

bool JsonParser::isObject(Json* v)
{
    if (v && v->type == JSON_OBJECT)
        return true;
    return false;
}

bool JsonParser::isTrue(Json* v)
{
    if (v && v->type == JSON_TRUE)
        return true;
    return false;
}

bool JsonParser::isFalse(Json* v)
{
    if (v && v->type == JSON_FALSE)
        return true;
    return false;
}

bool JsonParser::isNull(Json* v)
{
    if (v && v->type == JSON_NULL)
        return true;
    return false;
}

bool JsonParser::isString(Json* v)
{
    if (v && v->type == JSON_STRING)
        return true;
    return false;
}

bool JsonParser::isNumber(Json* v)
{
    if (v && v->type == JSON_NUMBER)
        return true;
    return false;
}

const char* JsonParser::getString(Json* v)
{
    if (isString(v) && v->child && v->child->text)
        return v->child->text;
    return NULL;
}

const char* JsonParser::getValue(Json* v)
{
    if (v->child && v->child->text)
        return v->child->text;
    return NULL;
}

const char* JsonParser::toBuffer(Buffer& ret, Json* root)
{
    char* str = NULL;
    if (!root)
        return NULL;
    if (json_tree_to_string(root, &str) == JsonOk && str)
    {
        ret << str;
        free(str);
    }
    return reinterpret_cast<const char*>(ret.rbuf());
}

const char* JsonParser::toString(std::string& ret, Json* root)
{
    char* str = NULL;
    if (!root)
        return NULL;
    if (json_tree_to_string(root, &str) == JsonOk && str)
    {
        ret.append(str);
        free(str);
    }
    return ret.c_str();
}

const char* JsonParser::toString()
{
    if (!_dirty)
        return _strfmt.c_str();
    toString(_strfmt, _root);
    _dirty = false;
    return _strfmt.c_str();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

