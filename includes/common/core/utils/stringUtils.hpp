/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file stringUtils.hpp
 * @brief String utility functions for common string operations.
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

/**
 * @brief Converts a value to its string representation.
 *
 * @tparam T The type of the value to convert.
 * @param value The value to convert to a string.
 * @return String representation of the value.
 */
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
