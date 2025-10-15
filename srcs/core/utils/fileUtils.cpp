// TODO: Don't forget to add 42 header !

/**
 * @file fileUtils.cpp
 * @brief
 */

#include <string>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief 
 *
 * @param filename 
 * @param ext 
 * @return 
 */
bool	hasExtension(const std::string &filename, const std::string &ext)
{
	return (
		filename.size() >= ext.size() 
		&& filename.substr(filename.size() - ext.size()) == ext
	);
}

/**
 * @brief 
 *
 * @param filename 
 * @param ext 
 * @return 
 */
std::string	filenameNoExt(const std::string &filename, const std::string &ext)
{
	std::size_t pos = filename.rfind(ext); 
	if (pos != std::string::npos && pos == filename.size() - ext.size())
		return (filename.substr(0, pos));
	return (filename);
}

/**
 * @brief 
 *
 * @param path 
 * @return 
 */
std::string	filenameNoPath(const std::string &path)
{
	std::size_t pos = path.rfind('/');
	if (pos == std::string::npos)
		return (path);
	return (path.substr(pos + 1));
}

} // !utils
} // !core
} // !common
