/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATcpSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/02/02 17:22:58 by blucken          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ATcpSocket.cpp
 * @brief 
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
 * @brief 
 */
ATcpSocket::ATcpSocket() : ASocket() {}

/**
 * @brief 
 *
 * @param init_fd
 */
ATcpSocket::ATcpSocket(int init_fd) : ASocket(init_fd) {}

/**
 * @brief 
 *
 * @param init_domain
 * @param init_protocol
 */
ATcpSocket::ATcpSocket(int init_domain, int init_protocol, bool isNonblock): ASocket(init_domain, SOCK_STREAM, init_protocol, isNonblock) {}

/**
 * @brief 
 */
ATcpSocket::~ATcpSocket() {}

/**
 * @brief 
 *
 * @param rhs
 */
ATcpSocket::ATcpSocket(const ATcpSocket &rhs) : ASocket(rhs) {}

/**
 * @brief 
 *
 * @param rhs
 * @return
 */
ATcpSocket &ATcpSocket::operator=(const ATcpSocket &rhs)
{
	ASocket::operator=(rhs);
	return (*this);
}

/**
 * @brief 
 *
 * @param buffer
 * @param length
 * @param flags 
 * @return
 */
ssize_t	ATcpSocket::recv(void *buffer, std::size_t length, int flags) const
{
	ssize_t rd;
	if ((rd = ::recv(_fd->get(), buffer, length, flags)) == -1) //FIXME: EAGAIN ET EWOULDBLOCK doivent pas throw
		throw std::runtime_error("recv failed: " + std::string(std::strerror(errno)));
	return (rd);
}

/**
 * @brief 
 *
 * @param buffer
 * @param length
 * @param flags
 * @return
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
