/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/13                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_TCPSERVER_HPP
#define COMMON_TCPSERVER_HPP

/**
 * @file TcpServer.hpp
 * @brief 
 */

#include <cerrno>
#include <common/core/net/sockets/ATcpSocket.hpp>
#include <common/core/net/sockets/TcpClient.hpp>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <utility>

namespace common
{
namespace core
{
namespace net
{

/**
 * @class TcpServer
 * @brief 
 */
class TcpServer : public ATcpSocket
{
	public:
		TcpServer();
		explicit TcpServer(int init_fd);
		TcpServer(int init_domain, int init_protocol, bool isNonblock = false);
		~TcpServer();

		TcpServer(const TcpServer &rhs);
		TcpServer &operator=(const TcpServer &rhs);

		void	listen(int backlog = SOMAXCONN);

		/**
		 * @brief 
		 *
		 * @tparam T
		 * @return
		 */
		   template<typename T>
		   std::pair<TcpClient, T> accept()
		   {
			   T addr = {};
			   socklen_t len = sizeof(T);
			   int cfd = ::accept(_fd->get(), reinterpret_cast<struct sockaddr *>(&addr), &len);
			   if (cfd == -1)
				   throw std::runtime_error("accept failed: " + std::string(std::strerror(errno)));
			   TcpClient cs(cfd);
			   return std::make_pair(cs, addr);
		   }
};

} // !net
} // !core
} // !common

#endif // !COMMON_TCPSERVER_HPP

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
