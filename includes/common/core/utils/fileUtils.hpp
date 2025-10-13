// TODO: Don't forget to add 42 header !

/**
 * @file fileUtils.hpp
 * @brief
 */

#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

namespace common
{
namespace core
{
namespace utils 
{

bool		hasExtension(const std::string &filename, const std::string &ext);
std::string	filenameNoExt(const std::string &filename, const std::string &ext);
std::string	filenameNoPath(const std::string &path);

} // !utils
} // !core
} // !common

#endif // !FILEUTILS_HPP
