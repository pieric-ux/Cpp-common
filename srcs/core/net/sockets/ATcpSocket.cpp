/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATcpSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/15                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ATcpSocket.cpp
 * @brief Implementation of abstract TCP socket base class.
 */

#include <cerrno>
#include <common/core/net/sockets/ASocket.hpp>
#include <common/core/net/sockets/ATcpSocket.hpp>
#include <stdexcept>
#include <sys/socket.h>

namespace  common
{
namespace core
{
namespace net
{


/**
 * @brief Default constructor. Creates an invalid TCP socket.
 */
ATcpSocket::ATcpSocket() : ASocket() {}

/**
 * @brief Constructor from an existing socket file descriptor.
 *
 * @param init_fd Existing socket file descriptor.
 */
ATcpSocket::ATcpSocket(int init_fd) : ASocket(init_fd) {}

/**
 * @brief Constructor creating a new TCP socket.
 *
 * @param init_domain Address family (AF_INET or AF_INET6).
 * @param init_protocol Protocol number (typically IPPROTO_TCP).
 * @param isNonblock Whether to set socket as non-blocking.
 */
ATcpSocket::ATcpSocket(int init_domain, int init_protocol, bool isNonblock): ASocket(init_domain, SOCK_STREAM, init_protocol, isNonblock) {}

/**
 * @brief Virtual destructor for polymorphic cleanup.
 */
ATcpSocket::~ATcpSocket() {}

/**
 * @brief Copy constructor.
 *
 * @param rhs Socket to copy from.
 */
ATcpSocket::ATcpSocket(const ATcpSocket &rhs) : ASocket(rhs) {}

/**
 * @brief Assignment operator.
 *
 * @param rhs Socket to assign from.
 * @return Reference to this socket.
 */
ATcpSocket &ATcpSocket::operator=(const ATcpSocket &rhs)
{
	ASocket::operator=(rhs);
	return (*this);
}

/**
 * @brief Receives data from the socket.
 *
 * @param buffer Buffer to store received data.
 * @param length Maximum number of bytes to receive.
 * @param flags Receive flags (default: 0).
 * @return Number of bytes received.
 * @throw std::runtime_error If recv fails.
 */
ssize_t	ATcpSocket::recv(void *buffer, std::size_t length, int flags) const
{
	ssize_t rd;
	if ((rd = ::recv(_fd->get(), buffer, length, flags)) == -1)
		throw std::runtime_error("recv failed: " + std::string(std::strerror(errno)));
	return (rd);
}

/**
 * @brief Sends data through the socket.
 *
 * @param buffer Data to send.
 * @param length Number of bytes to send.
 * @param flags Send flags (default: 0).
 * @return Number of bytes sent.
 * @throw std::runtime_error If send fails.
 */
ssize_t ATcpSocket::send(const void *buffer, std::size_t length, int flags) const
{
	ssize_t rd;
	if ((rd = ::send(_fd->get(), buffer, length, flags)) == -1)
		throw std::runtime_error("send failed: " + std::string(std::strerror(errno)));
	return (rd);
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
