// TODO: Don't forget to add 42 header !

/**
 * @file timeUtils.hpp
 * @brief
 */

#ifndef TIMEUTILS_HPP
#define TIMEUTILS_HPP

#include <string>
#include <ctime>

namespace utils
{

std::time_t nowSec();
long 		nowMilli();
double		relativeSec(const clock_t &startTime);
std::string	timestamp(const time_t &time);

} // !utils

#endif // !TIMEUTILS_HPP
