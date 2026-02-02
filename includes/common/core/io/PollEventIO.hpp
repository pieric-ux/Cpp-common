/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollEventIO.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/02/02 16:27:38 by blucken          ###   ########.fr       */
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
 * @brief 
 */


namespace common
{
namespace core
{
namespace io
{

/**
 * @class PollEventIO
 * @brief [TODO:description]
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