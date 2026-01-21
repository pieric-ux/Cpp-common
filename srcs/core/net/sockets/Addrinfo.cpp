/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Addrinfo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/15                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file Addrinfo.cpp
 * @brief 
 */

#include <common/core/raii/Deleters.hpp>
#include <common/core/raii/SharedPtr.hpp>
#include <common/core/net/sockets/Addrinfo.hpp>
#include <netdb.h>
#include <string>

namespace common
{
namespace core
{
namespace net
{

/**
 * @brief 
 */
Addrinfo::Addrinfo() : _res(NULL) {}

/**
 * @brief 
 *
 * @param hostname
 * @param servname
 * @param ai_flags
 * @param ai_family
 * @param ai_socktype
 * @param ai_protocol
 */
Addrinfo::Addrinfo(const char *hostname, const char *servname,
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
 * @brief 
 */
Addrinfo::~Addrinfo() {}

/**
 * @brief 
 *
 * @param rhs
 */
Addrinfo::Addrinfo(const Addrinfo &rhs) : _res(rhs._res) {}

/**
 * @brief 
 *
 * @param rhs
 * @return
 */
Addrinfo &Addrinfo::operator=(const Addrinfo &rhs)
{
	if (this != &rhs)
		_res = rhs._res;
	return (*this);
}

/**
 * @brief 
 *
 * @return
 */
struct addrinfo *Addrinfo::getRes() const
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
