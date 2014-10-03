
#include "File_POSIX.h"
#include "Buffer.h"
#include "Path.h"

#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>
#include <cstring>

namespace buf
{

FileImpl::FileImpl() : _fd(-1) {}

FileImpl::FileImpl(const std::string& path): _path(path), _fd(-1)
{
	std::string::size_type l = _path.size();
	if (l > 1 && _path[l-1] == '/')
		_path.resize(l-1);
}

FileImpl::~FileImpl()
{
    closeImpl();
}

bool FileImpl::openImpl(short rw)
{
    int flags = O_RDONLY;
    if (rw == FILE_RIMPL)
        flags = O_RDONLY;
    if (rw == FILE_WIMPL)
        flags = O_WRONLY;
    if (rw == FILE_RWIMPL)
        flags = O_RDWR;

    if (rw == FILE_RIMPL && !canReadImpl())
        return false;
    if (rw == FILE_WIMPL && !canWriteImpl())
        return false;
    if (rw == FILE_RWIMPL && (!canReadImpl() || !canWriteImpl()))
        return false;

    _fd = ::open(_path.c_str(), flags|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (_fd < 0)
        return false;

    return true;
}

bool FileImpl::openedImpl()
{
    return _fd != -1;
}

int FileImpl::readImpl(void* buf, size_t count)
{
    if (_fd < 0)
        return 0;
    return ::read(_fd, buf, count);
}

int FileImpl::writeImpl(const void* buf, size_t count)
{
    if (_fd < 0)
        return 0;
    return ::write(_fd, buf, count);
}

void FileImpl::closeImpl()
{
    if (_fd > 0)
        ::close(_fd);
    _fd = -1;
}

void FileImpl::setPathImpl(const std::string& path)
{
	_path = path;
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}

bool FileImpl::existsImpl() const
{
    if (_path.empty())
        return false;
	struct stat st;
	return stat(_path.c_str(), &st) == 0;
}

bool FileImpl::canReadImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (geteuid() == 0)
			return true;
		else if (st.st_uid == geteuid())
			return (st.st_mode & S_IRUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IRGRP) != 0;
		else
			return (st.st_mode & S_IROTH) != 0;
	}
	return false;
}


bool FileImpl::canWriteImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (geteuid() == 0)
			return true;
		else if (st.st_uid == geteuid())
			return (st.st_mode & S_IWUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IWGRP) != 0;
		else
			return (st.st_mode & S_IWOTH) != 0;
	}
	return false;
}


bool FileImpl::canExecuteImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid() || geteuid() == 0)
			return (st.st_mode & S_IXUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IXGRP) != 0;
		else
			return (st.st_mode & S_IXOTH) != 0;
	}
	return false;
}

bool FileImpl::isFileImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISREG(st.st_mode);
	return false;
}


bool FileImpl::isDirectoryImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISDIR(st.st_mode);
	return false;
}


bool FileImpl::isLinkImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (lstat(_path.c_str(), &st) == 0)
		return S_ISLNK(st.st_mode);
	return false;
}


bool FileImpl::isDeviceImpl() const
{
    if (_path.empty())
        return false;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
	return false;
}

bool FileImpl::isHiddenImpl() const
{
    if (_path.empty())
        return false;

	Path p(_path);
	p.makeFile();

	return p.getFileName()[0] == '.';
}

bool FileImpl::linkToImpl(const std::string& to)
{
#if _BSD_SOURCE || _XOPEN_SOURCE >= 500 || _POSIX_C_SOURCE >= 200112L || __APPLE__
    if (symlink(to.c_str(), _path.c_str()) != 0)
        return false;
#else
    if (link(to.c_str(), _path.c_str()) != 0)
        return false;
#endif
    return true;
}

Time FileImpl::createdImpl() const
{
    if (_path.empty())
        return Time();

#if 0
#if defined(__APPLE__) && defined(st_birthtime) && !defined(_NO_STAT64) // st_birthtime is available only on 10.5
	struct stat64 st;
	if (stat64(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_birthtime);
#elif defined(__FreeBSD__)
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_birthtime);
#else
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_ctime);
#endif 
#else
    // TODO:
#endif

	return Time();
}


Time FileImpl::getLastModifiedImpl() const
{
    if (_path.empty())
        return Time();

#if 0
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_mtime);
#else
    // TODO:
#endif

	return Time();
}


void FileImpl::setLastModifiedImpl(const Time& ts)
{
    if (_path.empty())
        return;

#if 0
	struct utimbuf tb;
	tb.actime  = ts.epochTime();
	tb.modtime = ts.epochTime();
	if (utime(_path.c_str(), &tb) != 0)
		handleLastErrorImpl(_path);
#else
    // TODO:
#endif
}


FileImpl::FileSizeImpl FileImpl::getSizeImpl() const
{
    if (_path.empty())
        return 0;

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return st.st_size;
	else
		handleLastErrorImpl(_path);
	return 0;
}

void FileImpl::setSizeImpl(FileSizeImpl size)
{
    if (_path.empty())
        return;

	if (truncate(_path.c_str(), size) != 0)
		handleLastErrorImpl(_path);
}


void FileImpl::setWriteableImpl(bool flag)
{
    if (_path.empty())
        return;

	struct stat st;
	if (stat(_path.c_str(), &st) != 0) 
		handleLastErrorImpl(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IWUSR;
	}
	else
	{
		mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0) 
		handleLastErrorImpl(_path);
}


void FileImpl::setExecutableImpl(bool flag)
{
    if (_path.empty())
        return;

	struct stat st;
	if (stat(_path.c_str(), &st) != 0) 
		handleLastErrorImpl(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IXUSR;
	}
	else
	{
		mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0) 
		handleLastErrorImpl(_path);
}


void FileImpl::copyToImpl(const std::string& path) const
{
    if (_path.empty())
        return;

	int sd = ::open(_path.c_str(), O_RDONLY);
	if (sd == -1) handleLastErrorImpl(_path);

	struct stat st;
	if (fstat(sd, &st) != 0) 
	{
		close(sd);
		handleLastErrorImpl(_path);
	}
	const long blockSize = st.st_blksize;

	int dd = ::open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode & S_IRWXU);
	if (dd == -1)
	{
		close(sd);
		handleLastErrorImpl(path);
	}
	Buffer buffer(blockSize);
	try
	{
		int n;
		while ((n = ::read(sd, buffer.wbuf(), blockSize)) > 0)
		{
			if (::write(dd, buffer.rbuf(), n) != n) 
				handleLastErrorImpl(path);
            buffer.rflip(buffer.rsize());
		}
		if (n < 0)
			handleLastErrorImpl(_path);
	}
	catch (...)
	{
		close(sd);
		close(dd);
		throw;
	}
	close(sd);
	if (fsync(dd) != 0) 
	{
		close(dd);
		handleLastErrorImpl(path);
	}
	if (close(dd) != 0)
		handleLastErrorImpl(path);
}


void FileImpl::renameToImpl(const std::string& path)
{
    if (_path.empty())
        return;

	if (rename(_path.c_str(), path.c_str()) != 0)
		handleLastErrorImpl(_path);
}


bool FileImpl::removeImpl()
{
    if (_path.empty())
        return false;

	int rc;
	if (!isLinkImpl() && isDirectoryImpl())
		rc = rmdir(_path.c_str());
	else
		rc = unlink(_path.c_str());
	//if (rc) handleLastErrorImpl(_path);
    return !rc;
}


bool FileImpl::createFileImpl()
{
    if (_path.empty())
        return false;
	
	int n = ::open(_path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (n == -1 && errno == EEXIST)
		return false;
	else
		handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::createDirectoryImpl()
{
    if (_path.empty())
        return false;

	if (existsImpl() && isDirectoryImpl())
		return false;
	if (mkdir(_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) 
		handleLastErrorImpl(_path);
	return true;
}


void FileImpl::handleLastErrorImpl(const std::string& path)
{
#if 0
	switch (errno)
	{
	case EIO:
		throw IOException(path);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path);
	case EACCES:
		throw FileAccessDeniedException(path);
	case ENOENT:
		throw FileNotFoundException(path);
	case ENOTDIR:
		throw OpenFileException("not a directory", path);
	case EISDIR:
		throw OpenFileException("not a file", path);
	case EROFS:
		throw FileReadOnlyException(path);
	case EEXIST:
		throw FileExistsException(path);
	case ENOSPC:
		throw FileException("no space left on device", path);
	case EDQUOT:
		throw FileException("disk quota exceeded", path);
#if !defined(_AIX)
	case ENOTEMPTY:
		throw FileException("directory not empty", path);
#endif
	case ENAMETOOLONG:
		throw PathSyntaxException(path);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path);
	default:
		throw FileException(std::strerror(errno), path);
	}
#else
    // TODO:
#endif
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

