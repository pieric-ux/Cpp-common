/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/12                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_ASOCKET_HPP
#define COMMON_ASOCKET_HPP

/**
 * @file ASocket.hpp
 * @brief 
 */

#include <common/core/net/sockets/ISocket.hpp>
#include <common/core/raii/UniquePtr.hpp>
#include <cerrno>
#include <cstring>
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
 * @class ASocket
 * @brief 
 */
class ASocket : public ISocket
{
	public:
		ASocket();
		explicit ASocket(int init_fd);
		ASocket(int domain, int type, int protocol, bool isNonblock);
		virtual ~ASocket() = 0;

		ASocket(const ASocket &rhs);
		ASocket &operator=(const ASocket &rhs);

		void	bind(const struct sockaddr *addr, socklen_t addrlen);
		void	close();
		int		getFd() const;
		bool	getIsNonblock() const;
		void	setIsNonblock(bool isNonblock);
		void	shutdown(int how = SHUT_RDWR);

		/**
		 * @brief 
		 *
		 * @tparam T
		 * @return
		 */
		template<typename T>
		T	getsockname() const
		{
			T addr = {};
			socklen_t len = sizeof(T);
			if (::getsockname(_fd->get(), reinterpret_cast<struct sockaddr *>(&addr), &len) == -1)
				throw std::runtime_error("getsockname failed: " + std::string(std::strerror(errno)));
			return (addr);
		}

		/**
		 * @brief 
		 *
		 * @tparam T
		 * @return
		 */
		template<typename T>
		T	getpeername() const
		{
			T addr = {};
			socklen_t len = sizeof(T);
			if (::getpeername(_fd->get(), reinterpret_cast<struct sockaddr *>(&addr), &len) == -1)
				throw std::runtime_error("getpeername failed: " + std::string(std::strerror(errno)));
			return (addr);
		}

		/**
		 * @brief 
		 *
		 * @tparam T
		 * @param optname
		 * @param level
		 * @return
		 */
		template<typename T>
		T	getsockopt(int optname, int level = SOL_SOCKET) const
		{
			T optval = {};
			socklen_t len = sizeof(T);
			if (::getsockopt(_fd->get(), level, optname, &optval, &len) == -1)
				throw std::runtime_error("getsockopt failed: " + std::string(std::strerror(errno)));
			return (optval);
		}

		/**
		 * @brief 
		 *
		 * @tparam T
		 * @param optname
		 * @param optval
		 * @param level
		 */
		template<typename T>
		void	setsockopt(int optname, const T &optval, int level = SOL_SOCKET)
		{
			if (::setsockopt(_fd->get(), level, optname, &optval, sizeof(T)) == -1)
				throw std::runtime_error("setsockopt failed: " + std::string(std::strerror(errno)));
		}

	protected:
		/**
		 * @class SocketFdRAII
		 * @brief 
		 *
		 */
		class SocketFdRAII
		{
			public:
				explicit SocketFdRAII(int init_fd);
				~SocketFdRAII();

				int get() const;
				void set(int new_fd);
				
			private:
				SocketFdRAII(const SocketFdRAII &rhs);
				SocketFdRAII &operator=(const SocketFdRAII &rhs);

				int _fdRef;
		};

		common::core::raii::UniquePtr<SocketFdRAII> _fd;
		bool										_isNonblock;

	private:
		int	getFlags() const;
};

bool	operator==(const ASocket &lhs, const ASocket &rhs);
bool	operator!=(const ASocket &lhs, const ASocket &rhs);
bool	operator<(const ASocket &lhs, const ASocket &rhs);
bool	operator>(const ASocket &lhs, const ASocket &rhs);
bool	operator<=(const ASocket &lhs, const ASocket &rhs);
bool	operator>=(const ASocket &lhs, const ASocket &rhs);

} // !net
} // !core
} // !common

#endif // !COMMON_ASOCKET_HPP

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
