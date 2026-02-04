/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/15                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ASocket.cpp
 * @brief Implementation of abstract socket base class.
 */

#include <common/core/net/sockets/ASocket.hpp>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace common
{
namespace core
{
namespace net
{

/**
 * @brief RAII wrapper for socket file descriptor.
 *
 * Manages the lifecycle of a socket file descriptor with automatic cleanup.
 *
 * @param init_fd Initial socket file descriptor.
 */
ASocket::SocketFdRAII::SocketFdRAII(int init_fd) : _fdRef(init_fd) {}

/**
 * @brief Destructor. Closes the socket file descriptor if it's valid.
 */
ASocket::SocketFdRAII::~SocketFdRAII()
{
	if (_fdRef != -1)
	{
		::close(_fdRef);
		_fdRef = -1;
	}
}

/**
 * @brief Gets the socket file descriptor.
 *
 * @return The file descriptor value, or -1 if invalid.
 */
int	ASocket::SocketFdRAII::get() const
{
	return (_fdRef);
}

/**
 * @brief Sets a new socket file descriptor.
 *
 * @param new_fd The new file descriptor to store.
 */
void	ASocket::SocketFdRAII::set(int new_fd)
{
	_fdRef = new_fd;
}

/**
 * @brief Default constructor. Creates an invalid socket.
 */
ASocket::ASocket() : _fd(new SocketFdRAII(-1)), _isNonblock(false) {}

/**
 * @brief Constructor from an existing file descriptor.
 *
 * @param init_fd Existing socket file descriptor to manage.
 */
ASocket::ASocket(int init_fd) : _fd(new SocketFdRAII(init_fd))
{
	int flags = getFlags();

	_isNonblock = (flags & O_NONBLOCK);
}

/**
 * @brief Constructor creating a new socket.
 *
 * @param domain Address family (e.g., AF_INET, AF_INET6).
 * @param type Socket type (e.g., SOCK_STREAM, SOCK_DGRAM).
 * @param protocol Protocol number (typically 0).
 * @param isNonblock Whether to set the socket as non-blocking.
 * @throw std::runtime_error If socket creation fails.
 */
ASocket::ASocket(int domain, int type, int protocol, bool isNonblock) : _fd(new SocketFdRAII(-1))
{
	int fd = ::socket(domain, type, protocol);
	if (fd == -1)
		throw std::runtime_error("socket failed: " + std::string(std::strerror(errno)));
	_fd->set(fd);

	if (isNonblock)
		setIsNonblock(isNonblock);
}

/**
 * @brief Virtual destructor for polymorphic cleanup.
 */
ASocket::~ASocket() {}


/**
 * @brief Copy constructor. Transfers ownership of the file descriptor.
 *
 * @param rhs Socket to copy from.
 */
ASocket::ASocket(const ASocket &rhs) : _fd(const_cast<ASocket&>(rhs)._fd.release()), _isNonblock(rhs._isNonblock) {}

/**
 * @brief Assignment operator. Transfers ownership of the file descriptor.
 *
 * @param rhs Socket to assign from.
 * @return Reference to this socket.
 */
ASocket &ASocket::operator=(const ASocket &rhs)
{
	if (this != &rhs)
	{
		_fd.reset(const_cast<ASocket&>(rhs)._fd.release());
		_isNonblock = rhs._isNonblock;
	}
	return (*this);
}

/**
 * @brief Binds the socket to a local address.
 *
 * @param addr Local address to bind to.
 * @param addrlen Length of the address structure.
 * @throw std::runtime_error If bind fails.
 */
void	ASocket::bind(const struct sockaddr *addr, socklen_t addrlen)
{
	if (::bind(_fd->get(), addr, addrlen) == -1)
		throw std::runtime_error("bind failed: " + std::string(std::strerror(errno)));
}

/**
 * @brief Gets the socket file descriptor.
 *
 * @return The file descriptor value.
 */
int	ASocket::getFd() const
{
	return (_fd->get());
}

/**
 * @brief Checks if the socket is in non-blocking mode.
 *
 * @return True if non-blocking, false otherwise.
 */
bool	ASocket::getIsNonblock() const
{
	return (_isNonblock);
}

/**
 * @brief Sets the socket to non-blocking or blocking mode.
 *
 * @param isNonblock True for non-blocking, false for blocking.
 * @throw std::runtime_error If fcntl fails.
 */
void	ASocket::setIsNonblock(bool isNonblock)
{
	int flags = getFlags();

	if (!_isNonblock && isNonblock)
	{
		if (::fcntl(_fd->get(), F_SETFL, flags | O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl failed: " + std::string(std::strerror(errno)));
	}
	else if (_isNonblock && !isNonblock)
	{
		if (::fcntl(_fd->get(), F_SETFL, flags & ~O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl failed: " + std::string(std::strerror(errno)));
	}
	_isNonblock = isNonblock;
}

/**
 * @brief Closes the socket file descriptor.
 *
 * @throw std::runtime_error If close fails.
 */
void	ASocket::close()
{
	if (_fd->get() != -1)
		if (::close(_fd->get()) == -1)
			throw std::runtime_error("close failed " + std::string(std::strerror(errno)));
	_fd->set(-1);
}

/**
 * @brief Shuts down the socket connection.
 *
 * @param how How to shut down (SHUT_RD, SHUT_WR, or SHUT_RDWR).
 * @throw std::runtime_error If shutdown fails.
 */
void	ASocket::shutdown(int how)
{
	if (::shutdown(_fd->get(), how) == -1)
		throw std::runtime_error("shutdown failed: " + std::string(std::strerror(errno)));
}

/**
 * @brief Gets the socket file descriptor flags.
 *
 * @return File descriptor flags.
 * @throw std::runtime_error If fcntl fails.
 */
int	ASocket::getFlags() const
{
	int flags = fcntl(_fd->get(), F_GETFL);
	if (flags == -1)
		throw std::runtime_error("fcntl failed: " + std::string(std::strerror(errno)));

	return (flags);
}

/**
 * @brief Equality operator. Compares socket file descriptors.
 *
 * @param lhs Left-hand side socket.
 * @param rhs Right-hand side socket.
 * @return True if sockets have the same file descriptor.
 */
bool	operator==(const ASocket &lhs, const ASocket &rhs)
{
	return (lhs.getFd() == rhs.getFd());
}

/**
 * @brief Inequality operator.
 *
 * @param lhs Left-hand side socket.
 * @param rhs Right-hand side socket.
 * @return True if sockets have different file descriptors.
 */
bool	operator!=(const ASocket &lhs, const ASocket &rhs)
{
	return (!(lhs == rhs));
}

/**
 * @brief Less-than operator. Compares file descriptor values.
 *
 * @param lhs Left-hand side socket.
 * @param rhs Right-hand side socket.
 * @return True if lhs fd < rhs fd.
 */
bool	operator<(const ASocket &lhs, const ASocket &rhs)
{
	return (lhs.getFd() < rhs.getFd());
}

/**
 * @brief Greater-than operator.
 *
 * @param lhs Left-hand side socket.
 * @param rhs Right-hand side socket.
 * @return True if lhs fd > rhs fd.
 */
bool	operator>(const ASocket &lhs, const ASocket &rhs)
{
	return (rhs < lhs);
}

/**
 * @brief Less-than-or-equal operator.
 *
 * @param lhs Left-hand side socket.
 * @param rhs Right-hand side socket.
 * @return True if lhs fd <= rhs fd.
 */
bool	operator<=(const ASocket &lhs, const ASocket &rhs)
{
	return (!(lhs > rhs));
}

/**
 * @brief Greater-than-or-equal operator.
 *
 * @param lhs Left-hand side socket.
 * @param rhs Right-hand side socket.
 * @return True if lhs fd >= rhs fd.
 */
bool	operator>=(const ASocket &lhs, const ASocket &rhs)
{
	return (!(lhs < rhs));
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
