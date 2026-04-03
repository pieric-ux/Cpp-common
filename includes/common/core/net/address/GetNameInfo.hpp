/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetNameInfo.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/04/03                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_GETNAMEINFO_HPP
#define COMMON_GETNAMEINFO_HPP

/**
 * @file GetNameInfo.hpp
 * @brief Network address-to-name resolution utilities wrapping getnameinfo(3).
 */

#include <netdb.h>
#include <stdexcept>
#include <string>
#include <utility>

namespace common
{
namespace core
{
namespace net
{

/**
 * @brief Converts a socket address structure to human-readable host and service strings.
 *
 * Wraps getnameinfo(). By default uses NI_NUMERICHOST | NI_NUMERICSERV to return
 * numeric IP and port without DNS resolution.
 *
 * @tparam T Address structure type (e.g., sockaddr_in, sockaddr_in6, sockaddr_storage).
 * @param addr The address structure to convert.
 * @param flags getnameinfo() flags (default: NI_NUMERICHOST | NI_NUMERICSERV).
 * @return Pair of (host, service) strings.
 * @throw std::runtime_error If getnameinfo fails.
 */
template<typename T>
std::pair<std::string, std::string>	getNameInfo(const T &addr, int flags = NI_NUMERICHOST | NI_NUMERICSERV)
{
	char hbuff[NI_MAXHOST];
	char sbuff[NI_MAXSERV];

	int code = ::getnameinfo(reinterpret_cast<const struct sockaddr *>(&addr), sizeof(T), hbuff, NI_MAXHOST, sbuff, NI_MAXSERV, flags);
	if (code)
		throw std::runtime_error("getnameinfo failed: " + std::string(gai_strerror(code)));

	return std::make_pair(std::string(hbuff), std::string(sbuff));
}

/**
 * @brief Overload for addrinfo — uses ai_addr and ai_addrlen directly.
 *
 * @param ai The addrinfo structure to convert.
 * @param flags getnameinfo() flags (default: NI_NUMERICHOST | NI_NUMERICSERV).
 * @return Pair of (host, service) strings.
 * @throw std::runtime_error If getnameinfo fails.
 */
inline std::pair<std::string, std::string>	getNameInfo(const struct addrinfo &ai, int flags = NI_NUMERICHOST | NI_NUMERICSERV)
{
	char hbuff[NI_MAXHOST];
	char sbuff[NI_MAXSERV];

	int code = ::getnameinfo(ai.ai_addr, ai.ai_addrlen, hbuff, NI_MAXHOST, sbuff, NI_MAXSERV, flags);
	if (code)
		throw std::runtime_error("getnameinfo failed: " + std::string(gai_strerror(code)));

	return std::make_pair(std::string(hbuff), std::string(sbuff));
}

} // !net
} // !core
} // !common

#endif // !COMMON_GETNAMEINFO_HPP

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
