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

/* ************************************************************************** */
/*                                                                            */
/*                                MIT License                                 */
/*                                                                            */
/*   Copyright (c) 2025 Demont Pieric, Lucken Bénédict                        */
/*                                                                            */
/*   Permission is hereby granted, free of charge, to any person obtaining    */
/*   a copy of this software and associated documentation files (the          */
/*   "Software"), to deal in the Software without restriction, including      */
/*   without limitation the rights to use, copy, modify, merge, publish,      */
/*   distribute, sublicense, and/or sell copies of the Software, and to       */
/*   permit persons to whom the Software is furnished to do so, subject to    */
/*   the following conditions:                                                */
/*                                                                            */
/*   The above copyright notice and this permission notice shall be included  */
/*   in all copies or substantial portions of the Software.                   */
/*                                                                            */
/*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  */
/*   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               */
/*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   */
/*   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     */
/*   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     */
/*   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        */
/*   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   */
/*                                                                            */
/* ************************************************************************** */
