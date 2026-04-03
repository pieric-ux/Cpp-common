/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetAddrinfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/15                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file GetAddrinfo.cpp
 * @brief Implementation of address information wrapper.
 */

#include <common/core/raii/Deleters.hpp>
#include <common/core/raii/SharedPtr.hpp>
#include <common/core/net/address/GetAddrinfo.hpp>
#include <netdb.h>
#include <string>
#include <stdexcept>

namespace common
{
namespace core
{
namespace net
{

/**
 * @brief Default constructor. Initializes with NULL addrinfo.
 */
GetAddrinfo::GetAddrinfo() : _res(NULL) {}

/**
 * @brief Resolves the given hostname/service and stores the result.
 *
 * @param hostname Hostname or IP address string (may be NULL for wildcard).
 * @param servname Service name or port number string.
 * @param ai_flags getaddrinfo() flags (e.g. AI_PASSIVE, AI_NUMERICSERV).
 * @param ai_family Address family (e.g. AF_UNSPEC, AF_INET, AF_INET6).
 * @param ai_socktype Socket type (e.g. SOCK_STREAM).
 * @param ai_protocol Protocol (0 for auto-select).
 * @throw std::runtime_error If getaddrinfo() fails.
 */
GetAddrinfo::GetAddrinfo(const char *hostname, const char *servname,
				   int ai_flags, int ai_family,
				   int ai_socktype, int ai_protocol)
{
	struct addrinfo hints = {}, *res = NULL;
	int error;

	hints.ai_flags = ai_flags;
	hints.ai_family = ai_family;
	hints.ai_socktype = ai_socktype;
	hints.ai_protocol = ai_protocol;

	error = getaddrinfo(hostname, servname, &hints, &res);
	if (error)
		throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(error)));
	_res = common::core::raii::SharedPtr<struct addrinfo>(res, new raii::AddrinfoDeleter());
}

/**
 * @brief Destructor. Releases the shared addrinfo chain via SharedPtr.
 */
GetAddrinfo::~GetAddrinfo() {}

/**
 * @brief Copy constructor. Shares ownership of the addrinfo chain via SharedPtr.
 *
 * @param rhs Source object to copy from.
 */
GetAddrinfo::GetAddrinfo(const GetAddrinfo &rhs) : _res(rhs._res) {}

/**
 * @brief Copy assignment operator. Shares ownership of the addrinfo chain via SharedPtr.
 *
 * @param rhs Source object to assign from.
 * @return Reference to this object.
 */
GetAddrinfo &GetAddrinfo::operator=(const GetAddrinfo &rhs)
{
	if (this != &rhs)
		_res = rhs._res;
	return (*this);
}

/**
 * @brief Returns the head of the resolved addrinfo linked list.
 *
 * @return Pointer to the first addrinfo entry, or NULL if not resolved.
 */
struct addrinfo *GetAddrinfo::getRes() const
{
	return (_res.get());
}

} // !net
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
