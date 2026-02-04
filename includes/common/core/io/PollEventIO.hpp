/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollEventIO.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/26                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_POLLEVENTIO_HPP
#define COMMON_POLLEVENTIO_HPP

#include <map>
#include <vector>
#include <poll.h>
#include <common/core/io/IEventIO.hpp>

/**
 * @file PollEventIO.hpp
 * @brief IEventIO implementation based on poll(2).
 */


namespace common
{
namespace core
{
namespace io
{

/**
 * @class PollEventIO
 * @brief I/O event handler using the poll(2) system call.
 *
 * This implementation uses poll(2) to monitor multiple file descriptors.
 * Poll is more efficient than select for a large number of descriptors and has no
 * FD_SETSIZE limit.
 *
 * @note More performant than select for a large number of descriptors.
 *
 * @startuml
 * class "PollEventIO" as PollEventIO [[classcommon_1_1core_1_1io_1_1_poll_event_i_o.html]] {
		- _events : map<int, e_Event>
		- _pollfds : vector<pollfd>
		--
		- eventToMask(event : e_Event) : short
		- maskToEvent(mask : short) : e_Event
		- rebuildPollFds() : void
		- updateEvents() : void
		+ PollEventIO()
		+ wait(timeout_ms : int) : int
		+ add(fd : int, mask : e_Event) : void
		+ remove(fd : int) : void
		+ update(fd : int, mask : e_Event) : void
		+ clear() : void
		+ getEvents(fd : int) : e_Event
	}
 * @enduml
 */
class PollEventIO : public IEventIO
{
	public:
		PollEventIO();
		~PollEventIO();

		int		wait(int timeout_ms);
		void	add(int fd, e_Event mask);
		void	remove(int fd);
		void	update(int fd, e_Event mask);
		void	clear();
		
		e_Event getEvents(int fd) const;

	private:
		PollEventIO(const PollEventIO &rhs);
		PollEventIO &operator=(const PollEventIO &rhs);

		short			eventToMask(e_Event event) const;
		e_Event			maskToEvent(short mask) const;
		void			rebuildPollFds();
		void			updateEvents();

		std::map<int, e_Event>		_events;
		std::vector<struct pollfd>	_pollfds;
};

} // !io
} // !core
} // !common

#endif // !COMMON_POLLEVENTIO_HPP

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
