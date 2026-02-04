/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATcpSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/13                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_ATCPSOCKET_HPP
#define COMMON_ATCPSOCKET_HPP

/**
 * @file ATcpSocket.hpp
 * @brief Abstract TCP socket implementation.
 */

#include <common/core/net/sockets/ASocket.hpp>

namespace common
{
namespace core
{
namespace net
{

/**
 * @class ATcpSocket
 * @brief Abstract base class for TCP socket operations.
 *
 * Extends ASocket with TCP-specific operations such as send and receive.
 * Provides a common interface for both TCP client and server implementations.
 *
 * @startuml
 * abstract class "ATcpSocket" as ATcpSocket {
		--
		+ ATcpSocket()
		+ ATcpSocket(init_fd : int)
		+ ATcpSocket(init_domain : int, init_protocol : int, isNonblock : bool)
		+ recv(buffer : void*, length : int, flags : int) : int
		+ send(buffer : const void*, length : int, flags : int) : int
	}
 * @enduml
 */
class ATcpSocket : public ASocket
{
	public:
		ATcpSocket();
		explicit ATcpSocket(int init_fd);
		ATcpSocket(int init_domain, int init_protocol, bool isNonblock);
		virtual ~ATcpSocket() = 0;

		ATcpSocket(const ATcpSocket &rhs);
		ATcpSocket &operator=(const ATcpSocket &rhs);

		ssize_t recv(void *buffer, std::size_t length, int flags = 0) const;
		ssize_t send(const void *buffer, std::size_t length, int flags = 0) const;
};

} // !net
} // !core
} // !common

#endif // !COMMON_ATCPSOCKET_HPP

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
