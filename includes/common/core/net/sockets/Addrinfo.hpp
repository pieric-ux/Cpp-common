/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Addrinfo.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/15                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_ADDRINFO_HPP
#define COMMON_ADDRINFO_HPP

/**
 * @file Addrinfo.hpp
 * @brief 
 */

#include <common/core/raii/SharedPtr.hpp>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace common
{
namespace core
{
namespace net
{

/**
 * @class Addrinfo
 * @brief 
 *
 * @startuml
 * class "Addrinfo" as Addrinfo {
		-  _res : addrinfo
		--
		+ Addrinfo()
		+ Addrinfo(hostname : string, servname : string, ai_flags : int, ai_family : int, ai_socktype : int, ai_protocol : int)
		+ getRes() : addrinfo
	}
 * @enduml
 */
class Addrinfo
{
	public:
		Addrinfo();
		Addrinfo(const char *hostname, const char *servname,
				int ai_flags = 0, int ai_family = 0,
				int ai_socktype = 0, int ai_protocol = 0);
		~Addrinfo();

		Addrinfo(const Addrinfo &rhs);
		Addrinfo &operator=(const Addrinfo &rhs);

		struct addrinfo *getRes() const;

	private:
		common::core::raii::SharedPtr<struct addrinfo> _res;
};

} // !net
} // !core
} // !common

#endif // !COMMON_ADDRINFO_HPP

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
