
#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include "json.h"
#include "Buffer.h"
#include "TypeTraits.h"
#include "Utils.h"

#include <string>

namespace buf
{

typedef json_t Json;
const int JsonOk = JSON_OK;

class JsonParser
{
public:
    JsonParser();
    ~JsonParser();

    void clear();

    bool parse(const char* ctx);
    bool parseFile(const char* filename);
    bool saveFile(const char* filename);

    // for roots
    inline Json* root()
    {
        if (!_root)
            _root = newObject();
        return _root;
    }
    inline Json* getRoot() { return root(); }

    Json* newObject(Json* parent = NULL);
    Json* newTrue(const char* name, Json* parent = NULL);
    Json* newFalse(const char* name, Json* parent = NULL);
    Json* newString(const char* name, const char* str, Json* parent = NULL);
    Json* newNull(const char* name, Json* parent = NULL);
    // XXX: element in array must be an object
    Json* newArray(const char* name, Json* parent = NULL);
    Json* newValue(const char* name, Json* value, Json* parent = NULL);

    bool addChild(Json* child, Json* parent = NULL);

    template <typename T>
    inline Json* newNumber(const char* name, T value, Json* parent = NULL)
    {
        if (!name)
            return NULL;
        char v[64] = {0};
        num2str(value, v, sizeof(v));
        Json* jvalue = json_new_number(v);
        if (!jvalue)
            return NULL;
        Json* r = newValue(name, jvalue, parent);
        if (!r)
        {
            json_free_value(&jvalue);
            return NULL;
        }
        return r;
    }

    Json* find(const char* name, Json* parent = NULL);

    bool isObject(Json* v);
    bool isTrue(Json* v);
    bool isFalse(Json* v);
    bool isNull(Json* v);
    bool isString(Json* v);
    bool isNumber(Json* v);

    const char* getString(Json* v);
    const char* getValue(Json* v);

    template <typename T>
    bool getNumber(Json* v, T& r)
    {
        const char* pv = getValue(v);
        if (pv)
        {
            str2num(pv, &r);
            return true;
        }
        return false;
    }

    const char* toString();

    static const char* toBuffer(Buffer& ret, Json* root = NULL);
    static const char* toString(std::string& ret, Json* root = NULL);

private:
    Json* _root;
    bool _dirty;
    std::string _strfmt;
};

} // namespace buf

#endif // JSONPARSER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

