
#ifndef MAIL_H_
#define MAIL_H_

#include "core/Config.h"
#include "core/UnorderedContainer.h"
#include "GUIDGenerator.h"
#include <string>

namespace buf
{

#define MAIL_TITLE_MAX 128
#define MAIL_CONTENT_MAX 8192

enum MailType
{
    MT_SYSTEM = 1,
    MT_PRIV = 2,
    MT_MAX
};

enum MailStatus
{
    MS_NEW          = 1,
    MS_OPENED       = 2,
    MS_DELETED      = 3,
    MS_MAX
};

enum MailContentType
{
    MCT_NORMAL = 1,
    MCT_ITEM,
    MCT_MAX
};

struct MailItem
{
    union
    {
        ItemID_t iid;
        EquipmentID_t oid;
        EquipmentID_t id;
    };
    ItemCount_t count;
};

class Mail
{
public:
    static Mail* newMail(const char* title, const char* content, u8_t type = MT_PRIV, const MailItem* mi);
    static Mail* newMail(std::string& title, std::string& content, u8_t type = MT_PRIV, const MailItem* mi);

    void setMailGUID(const GUID& guid);
    void pushItem(const MailItem* mi, size_t count);
    void clearItem();

private:
    Mail();

private:
    GUID _id;
    GUID _from;
    u8_t _type;
    u8_t _status;
    u8_t _contentType;
    u32_t _created;        // 创建时间
    u32_t _expiration;     // 过期时间
    std::string _title;
    std::string _content;
    std::vector<MailItem> _items;
};

class MailBox
{
public:
    typedef UNORDERED_MAP<GUID_t, const Mail*> container;

public:
    MailBox();
    ~MailBox();

    void post(const Mail* mail);
    const Mail* get(const GUID_t& mailid);
    void add(const Mail* mail);
    void remove(const Mail* mail);
    void remove(const GUID_t& mailid);
    void list(u32_t page, u32_t limit);
    void read(const GUID_t& mailid);
    void pick(const GUID_t& mailid);
    void notifyNew(const Mail* mail);

private:
    container _mails;
    // TODO: reject list
};

} // namespace buf

#endif // MAIL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

