/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectEventIO.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/26                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_SELECTEVENTIO_HPP
#define COMMON_SELECTEVENTIO_HPP

#include <map>
#include <sys/select.h>
#include <common/core/io/IEventIO.hpp>

/**
 * @file SelectEventIO.hpp
 * @brief IEventIO implementation based on select(2).
 */


namespace common
{
namespace core
{
namespace io
{

/**
 * @class SelectEventIO
 * @brief I/O event handler using the select(2) system call.
 *
 * This implementation uses select(2) to monitor multiple file descriptors.
 * Select is a portable mechanism but limited by FD_SETSIZE (typically 1024).
 *
 * @note Limited to FD_SETSIZE descriptors. Prefer poll for large sets.
 *
 * @startuml
 * class "SelectEventIO" as SelectEventIO {
		- _set : map<int, e_Event>
		- _nfds : int
		--
		- initSets(sets : Sets) : void
		- updateSets(sets : Sets) : void
		- initTimeout(timeout_ms : int) : timeval
		+ SelectEventIO()
		+ wait(timeout_ms : int) : int
		+ add(fd : int, mask : e_Event) : void
		+ remove(fd : int) : void
		+ update(fd : int, mask : e_Event) : void
		+ clear() : void
		+ getEvents(fd : int) : e_Event
	}
 * @enduml
 */
class SelectEventIO : public IEventIO
{
	public:
		SelectEventIO();
		~SelectEventIO();

		int		wait(int timeout_ms);
		void	add(int fd, e_Event mask);
		void	remove(int fd);
		void	update(int fd, e_Event mask);
		void	clear();
		
		e_Event getEvents(int fd) const;

	private:
		/**
		 * @class Sets
		 * @brief Structure grouping the three fd_set sets for select(2).
		 *
		 * @startuml
		 * struct "Sets" as Sets {
				fd_set read_fds
				fd_set write_fds
				fd_set except_fds
				--
				Sets()
			}
		 * @enduml
		 */
		struct Sets
		{
			fd_set _readfds;
			fd_set _writefds;
			fd_set _exceptfds;

			Sets();
		};

		SelectEventIO(const SelectEventIO &rhs);
		SelectEventIO &operator=(const SelectEventIO &rhs);

		void			initSets(Sets &sets) const;
		void			updateSets(Sets &sets);
		struct timeval	initTimeout(int timeout_ms);

		std::map<int, e_Event>	_set;
		int						_nfds;
};

} // !io
} // !core
} // !common

#endif // !COMMON_SELECTEVENTIO_HPP

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
