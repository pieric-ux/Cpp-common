/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:43:01 by blucken           #+#    #+#             */
/*   Updated: 2026/02/03 15:43:29 by blucken          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file
 * @brief 
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
 * @brief 
 *
 * @param init_fdRef
 */
ASocket::SocketFdRAII::SocketFdRAII(int init_fd) : _fdRef(init_fd) {}

/**
 * @brief 
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
 * @brief 
 *
 * @return
 */
int	ASocket::SocketFdRAII::get() const
{
	return (_fdRef);
}

/**
 * @brief 
 *
 * @param fd
 */
void	ASocket::SocketFdRAII::set(int new_fd)
{
	_fdRef = new_fd;
}

/**
 * @brief 
 */
ASocket::ASocket() : _fd(new SocketFdRAII(-1)), _isNonblock(false) {}

/**
 * @brief 
 *
 * @param init_fd
 */
ASocket::ASocket(int init_fd) : _fd(new SocketFdRAII(init_fd))
{
	int flags = getFlags();

	_isNonblock = (flags & O_NONBLOCK);
}

/**
 * @brief 
 *
 * @param domain 
 * @param type
 * @param protocol
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
 * @brief 
 */
ASocket::~ASocket() {}


/**
 * @brief 
 *
 * @param rhs
 */
ASocket::ASocket(const ASocket &rhs) : _fd(const_cast<ASocket&>(rhs)._fd.release()), _isNonblock(rhs._isNonblock) {}

/**
 * @brief 
 *
 * @param rhs
 * @return
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
 * @brief 
 *
 * @param addr
 */
void	ASocket::bind(const struct sockaddr *addr, socklen_t addrlen)
{
	if (::bind(_fd->get(), addr, addrlen) == -1)
		throw std::runtime_error("bind failed: " + std::string(std::strerror(errno)));
}

/**
 * @brief 
 *
 * @return
 */
int	ASocket::getFd() const
{
	return (_fd->get());
}

/**
 * @brief 
 *
 * @return
 */
bool	ASocket::getIsNonblock() const
{
	return (_isNonblock);
}

/**
 * @brief 
 *
 * @param isNonblock
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
 * @brief 
 */
void	ASocket::close()
{
	if (_fd->get() != -1)
		if (::close(_fd->get()) == -1)
			throw std::runtime_error("close failed " + std::string(std::strerror(errno)));
	_fd->set(-1);
}

/**
 * @brief 
 *
 * @param how
 */
void	ASocket::shutdown(int how)
{
	if (::shutdown(_fd->get(), how) == -1)
		throw std::runtime_error("shutdown failed: " + std::string(std::strerror(errno)));
}

/**
 * @brief 
 *
 * @return
 */
int	ASocket::getFlags() const
{
	int flags = fcntl(_fd->get(), F_GETFL);
	if (flags == -1)
		throw std::runtime_error("fcntl failed: " + std::string(std::strerror(errno)));

	return (flags);
}

/**
 * @brief 
 *
 * @param lhs
 * @param rhs
 * @return
 */
bool	operator==(const ASocket &lhs, const ASocket &rhs)
{
	return (lhs.getFd() == rhs.getFd());
}

/**
 * @brief 
 *
 * @param lhs
 * @param rhs
 * @return
 */
bool	operator!=(const ASocket &lhs, const ASocket &rhs)
{
	return (!(lhs == rhs));
}

/**
 * @brief 
 *
 * @param lhs
 * @param rhs
 * @return
 */
bool	operator<(const ASocket &lhs, const ASocket &rhs)
{
	return (lhs.getFd() < rhs.getFd());
}

/**
 * @brief 
 *
 * @param lhs
 * @param rhs
 * @return
 */
bool	operator>(const ASocket &lhs, const ASocket &rhs)
{
	return (rhs < lhs);
}

/**
 * @brief 
 *
 * @param lhs
 * @param rhs
 * @return
 */
bool	operator<=(const ASocket &lhs, const ASocket &rhs)
{
	return (!(lhs > rhs));
}

/**
 * @brief 
 *
 * @param lhs
 * @param rhs
 * @return
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
