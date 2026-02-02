/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollEventIO.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/02/02 23:18:23 by blucken          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common/core/io/IEventIO.hpp"
#include <algorithm>
#include <common/core/io/PollEventIO.hpp>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <poll.h>

/**
 * @file PollEventIO.cpp
 * @brief 
 *     #include <poll.h>
       int poll(struct pollfd *fds, nfds_t nfds, int timeout);
 */

namespace common
{
namespace core
{
namespace io
{

/**
 * @brief [TODO:description]
 */
PollEventIO::PollEventIO() : _events(), _pollfds() {}

/**
 * @brief [TODO:description]
 */
PollEventIO::~PollEventIO() {}

/**
 * @brief [TODO:description]
 *
 * @param timeout_ms [TODO:parameter]
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
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 * @param mask [TODO:parameter]
 */
void PollEventIO::add(int fd, e_Event mask)
{
	// TODO: check FD_SETSIZE?
	_events[fd] = mask;
}

/**
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 */
void PollEventIO::remove(int fd)
{
	std::map<int, e_Event>::iterator it = _events.find(fd);
	
	if (it != _events.end())
		_events.erase(it);
}

/**
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 * @param mask [TODO:parameter]
 */
void PollEventIO::update(int fd, e_Event mask)
{
	std::map<int, e_Event>::iterator it = _events.find(fd);
	if (it != _events.end())
		it->second = mask;
}

/**
 * @brief [TODO:description]
 */
void PollEventIO::clear()
{
	_events.clear();
	_pollfds.clear();
}

/**
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 * @return [TODO:return]
 */
IEventIO::e_Event PollEventIO::getEvents(int fd) const
{
	std::map<int, e_Event>::const_iterator it = _events.find(fd);
	if (it == _events.end())
		return (E_NONE);
	return (it->second);
}

/**
 * @brief [TODO:description]
 * 
 *     POLLIN There is data to read.

       POLLPRI //TODO: a inclure?
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

 * @param event [TODO:parameter]
 * @return [TODO:return]
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
 * @brief [TODO:description]
 *
 * @param mask [TODO:parameter]
 * @return [TODO:return]
 */
IEventIO::e_Event PollEventIO::maskToEvent(short mask) const
{
	e_Event event = static_cast<e_Event>(0);
	
	if (mask & POLLIN)
		event = static_cast<e_Event>(event | E_IN);
	if (mask & POLLOUT)
		event = static_cast<e_Event>(event | E_OUT);
	if (mask & (POLLERR | POLLHUP | POLLNVAL))
		event = static_cast<e_Event>(event | E_EXCEPT);
	
	return (event);
}

/**
 * @brief [TODO:description]
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
 * @brief [TODO:description]
 */
void PollEventIO::updateEvents()
{
	std::map<int, e_Event>::iterator it;

	for (it = _events.begin(); it != _events.end(); ++it)
		it->second = E_NONE;
	
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