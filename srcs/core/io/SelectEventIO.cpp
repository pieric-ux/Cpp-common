/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectEventIO.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/26                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common/core/io/IEventIO.hpp"
#include <algorithm>
#include <common/core/io/SelectEventIO.hpp>
#include <common/core/utils/stringUtils.hpp>
#include <cstring>
#include <cerrno>
#include <map>
#include <stdexcept>
#include <utility>
#include <sys/select.h>

/**
 * @file SelectEventIO.cpp
 * @brief Implementation of select(2)-based I/O event handler.
 */

namespace common
{
namespace core
{
namespace io
{

/**
 * @brief Constructor. Initializes all fd_set structures to zero.
 */
SelectEventIO::Sets::Sets()
{
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
	FD_ZERO(&_exceptfds);
}

/**
 * @brief Default constructor. Initializes empty event sets.
 */
SelectEventIO::SelectEventIO() : _set(), _nfds(0) {}

/**
 * @brief Destructor.
 */
SelectEventIO::~SelectEventIO() {}

/**
 * @brief Waits for events on monitored file descriptors.
 *
 * @param timeout_ms Timeout in milliseconds (-1 for infinite, 0 for non-blocking).
 * @return Number of file descriptors with events, or 0 on timeout.
 * @throw std::runtime_error If select fails.
 */
int	SelectEventIO::wait(int timeout_ms)
{
	struct timeval tv;
	Sets sets;
	int	ready;

	initSets(sets);
	tv = initTimeout(timeout_ms);
	if ((ready = ::select(_nfds, &sets._readfds, &sets._writefds, &sets._exceptfds, &tv)) == -1)
		throw std::runtime_error("select failed: " + std::string(std::strerror(errno)));
	if (ready)
		updateSets(sets);
	return (ready);
}

/**
 * @brief Adds a file descriptor to monitor for events.
 *
 * @param fd File descriptor to add.
 * @param mask Event mask to monitor (E_IN, E_OUT, E_EXCEPT).
 * @throw std::runtime_error If fd exceeds FD_SETSIZE.
 */
void SelectEventIO::add(int fd, e_Event mask)
{
	if (fd >= FD_SETSIZE)
		throw std::runtime_error("file descriptor exceeds limit " + common::core::utils::toString(FD_SETSIZE));
	if (fd >= _nfds)
		_nfds = fd + 1;
	_set[fd] = mask;
}

/**
 * @brief Removes a file descriptor from monitoring.
 *
 * @param fd File descriptor to remove.
 */
void SelectEventIO::remove(int fd)
{
	std::map<int, e_Event>::iterator it = _set.find(fd);
	if (it == _set.end())
		return ;
	_set.erase(it);
	if (_set.empty())
		_nfds = 0;
	else
		_nfds = std::max_element(_set.begin(), _set.end())->first + 1;
}

/**
 * @brief Updates the event mask for a monitored file descriptor.
 *
 * @param fd File descriptor to update.
 * @param mask New event mask.
 */
void SelectEventIO::update(int fd, e_Event mask)
{
	std::map<int, e_Event>::iterator it = _set.find(fd);
	if (it != _set.end())
		it->second = mask;
}

/**
 * @brief Removes all monitored file descriptors.
 */
void SelectEventIO::clear()
{
	_set.clear();
	_nfds = 0;
}

/**
 * @brief Gets the detected events for a file descriptor.
 *
 * @param fd File descriptor to query.
 * @return Detected event mask (E_NONE if fd not monitored).
 */
IEventIO::e_Event SelectEventIO::getEvents(int fd) const
{
	std::map<int, e_Event>::const_iterator it = _set.find(fd);
	if (it == _set.end())
		return (E_NONE);
	return (it->second);
}

/**
 * @brief Initializes fd_set structures based on monitored events.
 *
 * @param sets Sets structure to populate.
 */
void SelectEventIO::initSets(Sets &sets) const
{
	std::map<int, e_Event>::const_iterator it;
	for (it = _set.begin(); it != _set.end(); ++it)
	{
		if (it->second & E_IN)
			FD_SET(it->first, &sets._readfds);
		if (it->second & E_OUT)
			FD_SET(it->first, &sets._writefds);
		if (it->second & E_EXCEPT)
			FD_SET(it->first, &sets._exceptfds);
	}
}

/**
 * @brief Updates monitored events based on select(2) results.
 *
 * @param sets Populated Sets structure from select(2).
 */
void SelectEventIO::updateSets(Sets &sets)
{
	_set.clear();
	for (int fd = 0; fd < _nfds; ++fd)
	{
		e_Event mask = E_NONE;
		if (FD_ISSET(fd, &sets._readfds))
			mask = static_cast<e_Event>(mask | E_IN);
		if (FD_ISSET(fd, &sets._writefds))
			mask = static_cast<e_Event>(mask | E_OUT);
		if (FD_ISSET(fd, &sets._exceptfds))
			mask = static_cast<e_Event>(mask | E_EXCEPT);
		if (mask)
			_set[fd] = mask;
	}
}

/**
 * @brief Converts millisecond timeout to timeval structure.
 *
 * @param timeout_ms Timeout in milliseconds.
 * @return Initialized timeval structure.
 */
struct timeval SelectEventIO::initTimeout(int timeout_ms)
{
	struct timeval tv;

	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec = (timeout_ms % 1000) * 1000;

	return (tv);
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
