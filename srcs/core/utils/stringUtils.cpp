/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file stringUtils.cpp
 * @brief String utility functions for common operations.
 */

#include <cctype>
#include <string>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief Removes leading and trailing blank characters from a string.
 *
 * Trims whitespace (as determined by isblank) from both the beginning and end of the input string.
 *
 * @param str The string to trim.
 * @return A new string with leading and trailing blanks removed.
 */
std::string trim(const std::string &str)
{
	std::string::const_iterator	it = str.begin();
	while (it != str.end() && isblank(static_cast<unsigned char>(*it)))
		it++;

	if (it == str.end())
		return "";

	std::string::const_reverse_iterator rit = str.rbegin();
	while (rit != str.rend() && isblank(static_cast<unsigned char>(*rit)))
		rit++;

	return (std::string (it, rit.base()));
}

} // !utils
} // !core
} // !common
