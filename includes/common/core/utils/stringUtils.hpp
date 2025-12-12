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

#ifndef COMMON_STRINGUTILS_HPP
#define COMMON_STRINGUTILS_HPP

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

#endif // !COMMON_STRINGUTILS_HPP

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
