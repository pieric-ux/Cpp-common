/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniqueFd.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/04/08                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_UNIQUE_FD_HPP
#define COMMON_UNIQUE_FD_HPP

/**
 * @file UniqueFd.hpp
 * @brief Generic RAII wrapper around a POSIX file descriptor.
 */

#include <unistd.h>

namespace common
{
namespace core
{
namespace raii
{

/**
 * @brief Owning RAII guard for a single POSIX file descriptor.
 *
 * Holds an int directly (no heap allocation, no template, no deleter
 * indirection) and calls ::close on destruction iff the held value is
 * non-negative. A value of -1 acts as the empty / released sentinel,
 * making default construction and post-release destruction safe.
 *
 * Non-copyable: copy constructor and copy assignment are declared
 * private and intentionally not defined, expressing unique ownership
 * in C++98 without relying on move semantics. Use release() / reset()
 * / swap() to transfer ownership between guards.
 *
 * Intended for any POSIX fd source: open(), socket(), accept(),
 * pipe(), dup(), etc. All methods are throw().
 *
 * @startuml
 * class "UniqueFd" as UniqueFd {
		- _fd : int
		--
		+ UniqueFd(fd : int)
		+ get() : int
		+ valid() : bool
		+ release() : int
		+ reset(fd : int) : void
		+ swap(other : UniqueFd&) : void
	}
 * @enduml
 */
class UniqueFd
{
public:
	explicit UniqueFd(int fd = -1) throw() : _fd(fd) {}

	~UniqueFd() throw()
	{
		if (_fd >= 0)
			::close(_fd);
	}

	int		get() const throw()
	{
		return _fd;
	}
	
	void	set(int fd) throw()
	{
		if (_fd != fd)
		{
			if (_fd >= 0)
				::close(_fd);
			_fd = fd;
		}
	}
	
	bool	valid() const throw()
	{
		return _fd >= 0;
	}

	int		release() throw()
	{
		int fd = _fd;
		_fd = -1;
		return fd;
	}

	void	reset(int fd = -1) throw()
	{
		if (_fd != fd && _fd >= 0)
			::close(_fd);
		_fd = fd;
	}

	void	swap(UniqueFd &other) throw()
	{
		int tmp = _fd;
		_fd = other._fd;
		other._fd = tmp;
	}

private:
	int		_fd;

	UniqueFd(const UniqueFd &);
	UniqueFd &operator=(const UniqueFd &);
};

inline void swap(UniqueFd &a, UniqueFd &b) throw() { a.swap(b); }

} // !raii
} // !core
} // !common
#endif // !COMMON_UNIQUE_FD_HPP

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
