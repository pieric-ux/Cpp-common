/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/13                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file TcpClient.cpp
 * @brief Implementation of TCP client socket.
 */

#include "common/core/net/sockets/ATcpSocket.hpp"
#include <common/core/net/sockets/TcpClient.hpp>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

namespace common
{
namespace core
{
namespace net
{

/**
 * @brief Default constructor. Creates an invalid TCP client socket.
 */
TcpClient::TcpClient() : ATcpSocket() {}

/**
 * @brief Constructor from an existing socket file descriptor.
 *
 * @param init_fd Existing socket file descriptor.
 */
TcpClient::TcpClient(int init_fd) : ATcpSocket(init_fd) {}

/**
 * @brief Constructor creating a new TCP client socket.
 *
 * @param init_domain Address family (AF_INET or AF_INET6).
 * @param init_protocol Protocol number (typically IPPROTO_TCP).
 * @param isNonblock Whether to set socket as non-blocking.
 */
TcpClient::TcpClient(int init_domain, int init_protocol, bool isNonblock) : ATcpSocket(init_domain, init_protocol, isNonblock) {}

/**
 * @brief Destructor.
 */
TcpClient::~TcpClient() {}

/**
 * @brief Copy constructor.
 *
 * @param rhs Socket to copy from.
 */
TcpClient::TcpClient(const TcpClient &rhs) : ATcpSocket(rhs) {}

/**
 * @brief Assignment operator.
 *
 * @param rhs Socket to assign from.
 * @return Reference to this socket.
 */
TcpClient &TcpClient::operator=(const TcpClient &rhs)
{
	ATcpSocket::operator=(rhs);
	return (*this);
}

/**
 * @brief Establishes a connection to a remote server.
 *
 * @param addr Address of the remote server.
 * @param addrlen Length of the address structure.
 * @throw std::runtime_error If connection fails.
 */
void	TcpClient::connect(const struct sockaddr *addr, socklen_t addrlen)
{
	if (::connect(_fd->get(), addr, addrlen) == -1)
		throw std::runtime_error("connect failed: " + std::string(std::strerror(errno)));
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
