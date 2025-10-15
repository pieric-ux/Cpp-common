/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file timeUtils.cpp
 * @brief Utility functions for retrieving and manipulating time values.
 */ 

#include "sys/time.h"
#include <ctime>
#include <cstring>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief Get the current time in seconds since the Unix epoch.
 *
 * @return Current time as std::time_t (seconds since epoch).
 */
std::time_t	nowSec()
{
	return (std::time(NULL));
}

/**
 * @brief Get the current time in milliseconds.
 *
 * @return Current time in milliseconds (0-999) within the current second.
 */
long	nowMilli()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec / 1000);
}

/**
 * @brief Get the elapsed time in seconds since a given clock tick.
 *
 * @param startTime The starting clock tick (from std::clock()).
 * @return Elapsed time in seconds as a double.
 */
double	relativeSec(const clock_t &startTime)
{
	return ((static_cast<double>(std::clock()) - startTime) / CLOCKS_PER_SEC);
}

} // !utils
} // !core
} // !common
