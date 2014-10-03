
#ifndef FILE_H_
#define FILE_H_

#include "Config.h"
#include "RTime.h"
#include <vector>

#if defined(_OS_FAMILY_WINDOWS)
#include "win/File_WIN32.h"
#elif defined(_OS_FAMILY_UNIX)
#include "linux/File_POSIX.h"
#endif

namespace buf
{

#define FILE_R FILE_RIMPL
#define FILE_W FILE_WIMPL
#define FILE_RW (FILE_R|FILE_W)

class Path;

// The File class provides methods for working with a file.
class  File: private FileImpl
{
public:
	typedef FileSizeImpl FileSize;

	File();
	File(const std::string& path);
	File(const char* path);
	File(const Path& path);
	File(const File& file);
	virtual ~File();

    bool open(short rw = FILE_R);
    bool opened();

    int read(void* buf, size_t count);
    int write(const void* buf, size_t count);
    void close();

	const std::string& path() const;
	bool exists() const;
	bool canRead() const;
	bool canWrite() const;

    /**
     * Returns true if the file is executable.
     * On Windows and OpenVMS, the file must have
     * the extension ".EXE" to be executable.
     * On Unix platforms, the executable permission
     * bit must be set.
     */
	bool canExecute() const;

    // Returns true iff the file is a regular file.
	bool isFile() const;
	bool isLink() const;
	bool isDirectory() const;
	bool isDevice() const;
	bool isHidden() const;

    bool unLink();
    bool linkTo(const char* to);
    bool linkTo(const std::string& to);

    /**
     * Returns the creation date of the file.
     * Not all platforms or filesystems (e.g. Linux and most Unix
     * platforms with the exception of FreeBSD and Mac OS X)
     * maintain the creation date of a file.
     * On such platforms, created() returns
     * the time of the last inode modification.
     */
	Time created() const;

	Time getLastModified() const;
	void setLastModified(const Time& ts);
	FileSize getSize() const;
    inline FileSize size() const { return getSize(); }

    /**
	 * Sets the size of the file in bytes. Can be used
	 * to truncate a file.
     */
	void setSize(FileSize size);
		
	void setWriteable(bool flag = true);
		/// Makes the file writeable (if flag is true), or
		/// non-writeable (if flag is false) by setting the
		/// file's flags in the filesystem accordingly.
		
	void setReadOnly(bool flag = true);
		/// Makes the file non-writeable (if flag is true), or
		/// writeable (if flag is false) by setting the
		/// file's flags in the filesystem accordingly.
		
	void setExecutable(bool flag = true);
		/// Makes the file executable (if flag is true), or
		/// non-executable (if flag is false) by setting
		/// the file's permission bits accordingly.
		///
		/// Does nothing on Windows and OpenVMS.	
		
	void copyTo(const std::string& path) const;
		/// Copies the file (or directory) to the given path. 
		/// The target path can be a directory.
		///
		/// A directory is copied recursively.

	void moveTo(const std::string& path);
		/// Copies the file (or directory) to the given path and 
		/// removes the original file. The target path can be a directory.
		
	void renameTo(const std::string& path);
		/// Renames the file to the new name.
		
	void remove(bool recursive = false);
		/// Deletes the file. If recursive is true and the
		/// file is a directory, recursively deletes all
		/// files in the directory.
	
	bool createFile();
		/// Creates a new, empty file in an atomic operation.
		/// Returns true if the file has been created and false
		/// if the file already exists. Throws an exception if
		/// an error occurs.
	
	bool createDirectory();
		/// Creates a directory. Returns true if the directory
		/// has been created and false if it already exists.
		/// Throws an exception if an error occurs.
	
	void createDirectories();
		/// Creates a directory (and all parent directories
		/// if necessary).
		
	void list(std::vector<std::string>& files) const;
		/// Fills the vector with the names of all
		/// files in the directory.

	void list(std::vector<File>& files) const;
		/// Fills the vector with the names of all
		/// files in the directory.

	static void handleLastError(const std::string& path);
		/// For internal use only. Throws an appropriate
		/// exception for the last file-related error.

protected:
	void copyDirectory(const std::string& path) const;
		/// Copies a directory. Used internally by copyTo().
};

//
// inlines
//
inline const std::string& File::path() const
{
	return getPathImpl();
}

} // namespace buf

#endif // FILE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

