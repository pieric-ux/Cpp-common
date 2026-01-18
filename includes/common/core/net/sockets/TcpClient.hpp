/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/13                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_TCPCLIENT_HPP
#define COMMON_TCPCLIENT_HPP

/**
 * @file TcpClient.hpp
 * @brief 
 */

#include <common/core/net/sockets/ATcpSocket.hpp>

namespace common
{
namespace core
{
namespace net
{

/**
 * @class TcpClient
 * @brief 
 */
class TcpClient : public ATcpSocket
{
	public:
		TcpClient();
		explicit TcpClient(int init_fd);
		TcpClient(int init_domain, int init_protocol, bool isNonblock = false);
		~TcpClient();
		
		TcpClient(const TcpClient &rhs);
		TcpClient &operator=(const TcpClient &rhs);

		void	connect(const struct sockaddr *addr, socklen_t addrlen);
};

} // !net
} // !core
} // !common

#endif // !COMMON_TCPCLIENT_HPP

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
