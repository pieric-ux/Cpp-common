/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/12                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_ISOCKET_HPP
#define COMMON_ISOCKET_HPP

/**
 * @file ISocket.hpp
 * @brief 
 */

#include <sys/socket.h>

namespace common
{
namespace core
{
namespace net
{

/**
 * @class ISocket
 * @brief 
 *
 * @startuml
 * interface "ISocket" as ISocket {
		+ bind(addr : sockaddr, addrlen : socklen_t) : void
		+ close() : void
		+ getFd() : int
		+ shutdown(how : int) : void
	}
 * @enduml
 */
class ISocket
{
	public:
		virtual ~ISocket() {};

		virtual void	bind(const struct sockaddr *addr, socklen_t addrlen) = 0;
		virtual void	close() = 0;
		virtual int		getFd() const = 0;
		virtual void	shutdown(int how = SHUT_RDWR) = 0;
};

} // !net
} // !core
} // !common

#endif // !COMMON_ISOCKET_HPP

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
