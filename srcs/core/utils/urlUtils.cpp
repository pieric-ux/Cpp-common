/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/04/08                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include <cstdlib>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief Decode percent-encoded characters (%XX) in a URI path.
 *
 * @param encoded [TODO:parameter]
 * @return [TODO:return]
 */
std::string	urlDecode(const std::string &encoded)
{
	std::string result;
	result.reserve(encoded.size());

	std::string::const_iterator it = encoded.begin();
	for (; it != encoded.end(); ++it)
	{
		if (*it == '%' && std::distance(it, encoded.end()) >= 2)
		{
			char msb = *(it + 1);
			char lsb = *(it + 2);

			bool msbHex = (std::isdigit(msb) || std::isxdigit(msb));
			bool lsbHex = (std::isdigit(lsb) || std::isxdigit(lsb));

			if (msbHex && lsbHex)
			{
				char hex[3] = { msb, lsb, '\0' };
				result += static_cast<char>(std::strtol(hex, NULL, 16));
				it += 2;
				continue;
			}
		}
		result += *it;
	}

	return result;
}

} // !utils
} // !core
} // !common

/* ************************************************************************** */
/*                                                                            */
/*                                MIT License                                 */
/*                                                                            */
/*   Copyright (c) 2026 Demont Pieric, Lucken Bénédict                        */
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
