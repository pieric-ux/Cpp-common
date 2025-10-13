// TODO: Don't forget to add 42 header !

/**
 * @file stringUtils.hpp
 * @brief
 */

#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <sstream>

namespace common
{
namespace core
{
namespace utils 
{

std::string trim(const std::string &str);

template<typename T>
std::string	toString(const T &value)
{
	std::ostringstream oss;
	oss << value;
	return (oss.str());
}

} // !utils
} // !core
} // !common

#endif // !STRINGUTILS_HPP
