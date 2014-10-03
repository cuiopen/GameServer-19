
#include "Config.h"
#include "File.h"
#include "Path.h"

#if defined(_OS_FAMILY_WINDOWS)
#include "win/File_WIN32.cpp"
#elif defined(_OS_FAMILY_UNIX)
#include "linux/File_POSIX.cpp"
#endif

namespace buf
{

File::File() {}
File::File(const std::string& path): FileImpl(path) {}
File::File(const char* path): FileImpl(std::string(path)) {}
File::File(const Path& path): FileImpl(path.toString()) {}
File::File(const File& file): FileImpl(file.getPathImpl()) {}
File::~File() {}

bool File::open(short rw)
{
    return openImpl(rw);
}

bool File::opened()
{
    return openedImpl();
}

int File::read(void* buf, size_t count)
{
    return readImpl(buf, count);
}

int File::write(const void* buf, size_t count)
{
    return writeImpl(buf, count);
}

void File::close()
{
    closeImpl();
}

bool File::exists() const
{
	return existsImpl();
}
	
bool File::canRead() const
{
	return canReadImpl();
}
	
bool File::canWrite() const
{
	return canWriteImpl();
}

bool File::canExecute() const
{
	return canExecuteImpl();
}

bool File::isFile() const
{
	return isFileImpl();
}
	
bool File::isDirectory() const
{
	return isDirectoryImpl();
}

bool File::isLink() const
{
	return isLinkImpl();
}

bool File::isDevice() const
{
	return isDeviceImpl();
}

bool File::isHidden() const
{
	return isHiddenImpl();
}

bool File::unLink()
{
    return removeImpl();
}

bool File::linkTo(const char* to)
{
    if (!to)
        return false;
    return linkToImpl(to);
}

bool File::linkTo(const std::string& to)
{
    return linkToImpl(to);
}

Time File::created() const
{
	return createdImpl();
}

Time File::getLastModified() const
{
	return getLastModifiedImpl();
}

void File::setLastModified(const Time& ts)
{
	setLastModifiedImpl(ts);
}

File::FileSize File::getSize() const
{
	return getSizeImpl();
}

void File::setSize(FileSizeImpl size)
{
	setSizeImpl(size);
}

void File::setWriteable(bool flag)
{
	setWriteableImpl(flag);
}

void File::setReadOnly(bool flag)
{
	setWriteableImpl(!flag);
}

void File::setExecutable(bool flag)
{
	setExecutableImpl(flag);
}

void File::copyTo(const std::string& path) const
{
	Path src(getPathImpl());
	Path dest(path);
	File destFile(path);
	if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory())
	{
		dest.makeDirectory();
		dest.setFileName(src.getFileName());
	}
	if (isDirectory())
		copyDirectory(dest.toString());
	else
		copyToImpl(dest.toString());
}

void File::copyDirectory(const std::string& path) const
{
#if 0
	File target(path);
	target.createDirectories();

	Path src(getPathImpl());
	src.makeFile();
	DirectoryIterator it(src);
	DirectoryIterator end;
	for (; it != end; ++it)
	{
		it->copyTo(path);
	}
#else
    // TODO:
#endif
}

void File::moveTo(const std::string& path)
{
	copyTo(path);
	remove(true);
	setPathImpl(path);
}

void File::renameTo(const std::string& path)
{
	renameToImpl(path);
	setPathImpl(path);
}

void File::remove(bool recursive)
{
	if (recursive && !isLink() && isDirectory())
	{
		std::vector<File> files;
		list(files);
		for (std::vector<File>::iterator it = files.begin(); it != files.end(); ++it)
		{
			it->remove(true);
		}
	}
	removeImpl();
}

bool File::createFile()
{
	return createFileImpl();
}

bool File::createDirectory()
{
	return createDirectoryImpl();
}

void File::createDirectories()
{
	if (!exists())
	{
		Path p(getPathImpl());
		p.makeDirectory();
		if (p.depth() > 1)
		{
			p.makeParent();
			File f(p);
			f.createDirectories();
		}
		createDirectoryImpl();
	}
}

void File::list(std::vector<std::string>& files) const
{
#if 0
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(it.name());
		++it;
	}
#else
    // TODO:
#endif
}

void File::list(std::vector<File>& files) const
{
#if 0
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(*it);
		++it;
	}
#else
    // TODO:
#endif
}

void File::handleLastError(const std::string& path)
{
	handleLastErrorImpl(path);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

