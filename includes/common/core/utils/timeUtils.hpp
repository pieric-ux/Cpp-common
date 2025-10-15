/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file timeUtils.hpp
 * @brief Time utility functions for retrieving and formatting time values.
 */

#ifndef TIMEUTILS_HPP
#define TIMEUTILS_HPP

#include <string>
#include <ctime>

namespace common
{
namespace core
{
namespace utils
{

std::time_t nowSec();
long 		nowMilli();
double		relativeSec(const clock_t &startTime);
std::string	timestamp(const time_t &time);

} // !utils
} // !core
} // !common

#endif // !TIMEUTILS_HPP
