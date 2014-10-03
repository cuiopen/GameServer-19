
#ifndef PATH_POSIX_H_
#define PATH_POSIX_H_

#include <vector>
#include <string>

namespace buf
{

class PathImpl
{
public:
	static std::string currentImpl();
	static std::string homeImpl();
	static std::string tempImpl();
	static std::string nullImpl();
	static std::string expandImpl(const std::string& path);
	static void listRootsImpl(std::vector<std::string>& roots);
};

} // namespace buf

#endif // PATH_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

