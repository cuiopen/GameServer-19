
#ifndef FILE_POSIX_H_
#define FILE_POSIX_H_

#include "../Config.h"
#include "../RTime.h"

#include <string>

namespace buf
{

#define FILE_RIMPL      0x01
#define FILE_WIMPL      0x02
#define FILE_RWIMPL (FILE_RIMPL|FILE_WIMPL)

class FileImpl
{
protected:
	typedef u64_t FileSizeImpl;

	FileImpl();
	FileImpl(const std::string& path);

	virtual ~FileImpl();

    bool openImpl(short rw = FILE_RIMPL);
    bool openedImpl();
    int readImpl(void* buf, size_t count);
    int writeImpl(const void* buf, size_t count);
    void closeImpl();

	void setPathImpl(const std::string& path);
	const std::string& getPathImpl() const;

	bool existsImpl() const;
	bool canReadImpl() const;
	bool canWriteImpl() const;
	bool canExecuteImpl() const;

	bool isFileImpl() const;
	bool isDirectoryImpl() const;
	bool isLinkImpl() const;
	bool isDeviceImpl() const;
	bool isHiddenImpl() const;

    bool linkToImpl(const std::string& to);

	Time createdImpl() const;
	Time getLastModifiedImpl() const;

	void setLastModifiedImpl(const Time& ts);
	FileSizeImpl getSizeImpl() const;
	void setSizeImpl(FileSizeImpl size);
	void setWriteableImpl(bool flag = true);		
	void setExecutableImpl(bool flag = true);		

	void copyToImpl(const std::string& path) const;
	void renameToImpl(const std::string& path);
	bool removeImpl();

	bool createFileImpl();
	bool createDirectoryImpl();

	static void handleLastErrorImpl(const std::string& path);
	
private:
	std::string _path;
    int _fd;
};

//
// inlines
//
inline const std::string& FileImpl::getPathImpl() const
{
	return _path;
}

} // namespace buf

#endif // FILE_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

