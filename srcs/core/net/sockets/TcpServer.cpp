/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/13                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file TcpServer.cpp
 * @brief 
 */

#include <common/core/net/sockets/ATcpSocket.hpp>
#include <common/core/net/sockets/TcpServer.hpp>
#include <stdexcept>
#include <sys/socket.h>

namespace common
{
namespace core
{
namespace net
{

/**
 * @brief 
 */
TcpServer::TcpServer() : ATcpSocket() {}

/**
 * @brief 
 *
 * @param init_fd
 */
TcpServer::TcpServer(int init_fd) : ATcpSocket(init_fd) {}

/**
 * @brief 
 *
 * @param init_domain
 * @param init_protocol
 */
TcpServer::TcpServer(int init_domain, int init_protocol, bool isNonblock) : ATcpSocket(init_domain, init_protocol, isNonblock) {}

/**
 * @brief 
 */
TcpServer::~TcpServer() {}

/**
 * @brief 
 *
 * @param rhs
 */
TcpServer::TcpServer(const TcpServer &rhs) : ATcpSocket(rhs) {}

/**
 * @brief 
 *
 * @param rhs
 * @return
 */
TcpServer &TcpServer::operator=(const TcpServer &rhs)
{
	ATcpSocket::operator=(rhs);
	return (*this);
}

/**
 * @brief 
 *
 * @param backlog
 */
void	TcpServer::listen(int backlog)
{
	if (::listen(_fd->get(), backlog) == -1)
		throw std::runtime_error("listen failed: " + std::string(std::strerror(errno)));
}

} // !net
} // !core
} // !common

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
