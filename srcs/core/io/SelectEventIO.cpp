/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectEventIO.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:57:19 by blucken           #+#    #+#             */
/*   Updated: 2026/02/03 15:57:21 by blucken          ###   ########.fr       */
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
 * @brief 
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
SelectEventIO::Sets::Sets()
{
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
	FD_ZERO(&_exceptfds);
}

/**
 * @brief [TODO:description]
 */
SelectEventIO::SelectEventIO() : _set(), _nfds(0) {}

/**
 * @brief [TODO:description]
 */
SelectEventIO::~SelectEventIO() {}

/**
 * @brief [TODO:description]
 *
 * @param timeout_ms [TODO:parameter]
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
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 * @param mask [TODO:parameter]
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
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
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
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 * @param mask [TODO:parameter]
 */
void SelectEventIO::update(int fd, e_Event mask)
{
	std::map<int, e_Event>::iterator it = _set.find(fd);
	if (it != _set.end())
		it->second = mask;
}

/**
 * @brief [TODO:description]
 */
void SelectEventIO::clear()
{
	_set.clear();
	_nfds = 0;
}

/**
 * @brief [TODO:description]
 *
 * @param fd [TODO:parameter]
 * @return [TODO:return]
 */
IEventIO::e_Event SelectEventIO::getEvents(int fd) const
{
	std::map<int, e_Event>::const_iterator it = _set.find(fd);
	if (it == _set.end())
		return (E_NONE);
	return (it->second);
}

/**
 * @brief [TODO:description]
 *
 * @param sets [TODO:parameter]
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
 * @brief [TODO:description]
 *
 * @param sets [TODO:parameter]
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
 * @brief [TODO:description]
 *
 * @param timeout_ms [TODO:parameter]
 * @return [TODO:return]
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
