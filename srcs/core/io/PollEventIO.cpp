/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollEventIO.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/26                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common/core/io/IEventIO.hpp>
#include <common/core/io/PollEventIO.hpp>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <poll.h>

/**
 * @file PollEventIO.cpp
 * @brief Implementation of poll(2)-based I/O event handler.
 */

namespace common
{
namespace core
{
namespace io
{

/**
 * @brief Default constructor. Initializes empty poll structures.
 */
PollEventIO::PollEventIO() : _events(), _pollfds() {}

/**
 * @brief Destructor.
 */
PollEventIO::~PollEventIO() {}

/**
 * @brief Waits for events on monitored file descriptors.
 *
 * @param timeout_ms Timeout in milliseconds (-1 for infinite, 0 for non-blocking).
 * @return Number of file descriptors with events, or 0 on timeout.
 * @throw std::runtime_error If poll fails.
 */
int	PollEventIO::wait(int timeout_ms)
{
	int ready;

	if (_events.empty())
		return (0);
	rebuildPollFds();
	if ((ready = ::poll(&_pollfds[0], _pollfds.size(), timeout_ms)) == -1)
		throw std::runtime_error("poll failed: " + std::string(std::strerror(errno)));
	if (ready)
		updateEvents();
	return (ready);
}

/**
 * @brief Adds a file descriptor to monitor for events.
 *
 * @param fd File descriptor to add.
 * @param mask Event mask to monitor (E_IN, E_OUT, E_EXCEPT).
 */
void PollEventIO::add(int fd, e_Event mask)
{
	// TODO: check FD_SETSIZE?
	_events[fd] = mask;
}

/**
 * @brief Removes a file descriptor from monitoring.
 *
 * @param fd File descriptor to remove.
 */
void PollEventIO::remove(int fd)
{
	std::map<int, e_Event>::iterator it = _events.find(fd);
	
	if (it != _events.end())
		_events.erase(it);
}

/**
 * @brief Updates the event mask for a monitored file descriptor.
 *
 * @param fd File descriptor to update.
 * @param mask New event mask.
 */
void PollEventIO::update(int fd, e_Event mask)
{
	std::map<int, e_Event>::iterator it = _events.find(fd);
	if (it != _events.end())
		it->second = mask;
}

/**
 * @brief Removes all monitored file descriptors.
 */
void PollEventIO::clear()
{
	_events.clear();
	_pollfds.clear();
}

/**
 * @brief Gets the detected events for a file descriptor.
 *
 * @param fd File descriptor to query.
 * @return Detected event mask (E_NONE if fd not monitored).
 */
IEventIO::e_Event PollEventIO::getEvents(int fd) const
{
	std::map<int, e_Event>::const_iterator it = _events.find(fd);
	if (it == _events.end())
		return (E_NONE);
	return (it->second);
}

/**
 * @brief Converts IEventIO event mask to poll(2) event mask.
 * 
 *     POLLIN There is data to read.
              There is some exceptional condition on the file descriptor.
              Possibilities include:

              •  There is out-of-band data on a TCP socket (see tcp(7)).

              •  A pseudoterminal master in packet mode has seen a state
                 change on the slave (see ioctl_tty(2)).

              •  A cgroup.events file has been modified (see cgroups(7)).

       POLLOUT
              Writing is now possible, though a write larger than the
              available space in a socket or pipe will still block
              (unless O_NONBLOCK is set).

       POLLERR
              Error condition (only returned in revents; ignored in
              events).  This bit is also set for a file descriptor
              referring to the write end of a pipe when the read end has
              been closed.

       POLLHUP
              Hang up (only returned in revents; ignored in events).
              Note that when reading from a channel such as a pipe or a
              stream socket, this event merely indicates that the peer
              closed its end of the channel.  Subsequent reads from the
              channel will return 0 (end of file) only after all
              outstanding data in the channel has been consumed.
 */
short PollEventIO::eventToMask(e_Event event) const
{
	short mask = 0;
	
	if (event & E_IN)
		mask |= POLLIN;
	if (event & E_OUT)
		mask |= POLLOUT;
	if (event & E_EXCEPT)
		mask |= POLLERR | POLLHUP;
	
	return (mask);
}

/**
 * @brief Converts poll(2) event mask to IEventIO event mask.
 *
 * @param mask poll(2) event mask.
 * @return Corresponding IEventIO event mask.
 */
IEventIO::e_Event PollEventIO::maskToEvent(short mask) const
{
	e_Event event = E_NONE;
	
	if (mask & POLLIN)
		event = static_cast<e_Event>(event | E_IN);
	if (mask & POLLOUT)
		event = static_cast<e_Event>(event | E_OUT);
	if (mask & (POLLERR | POLLHUP | POLLNVAL))
		event = static_cast<e_Event>(event | E_EXCEPT);
	
	return (event);
}

/**
 * @brief Rebuilds the pollfd vector from the event map.
 */
void PollEventIO::rebuildPollFds()
{
	_pollfds.clear();

	std::map<int, e_Event>::const_iterator it;
	
	for (it = _events.begin(); it != _events.end(); ++it)
	{
		struct pollfd pfd;
		pfd.fd = it->first;
		pfd.events = eventToMask(it->second);
		pfd.revents = 0;
		_pollfds.push_back(pfd);
	}
}

/**
 * @brief Updates detected events from poll(2) results.
 */
void PollEventIO::updateEvents()
{
	std::map<int, e_Event>::iterator it;

	// for (it = _events.begin(); it != _events.end(); ++it)
	// 	it->second = E_NONE;
	
	for (size_t i = 0; i < _pollfds.size(); ++i)
	{
		if (_pollfds[i].revents != 0)
		{
			e_Event mask = maskToEvent(_pollfds[i].revents);
			if (mask)
				_events[_pollfds[i].fd] = mask;
		}
	}
}

} // !io
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
