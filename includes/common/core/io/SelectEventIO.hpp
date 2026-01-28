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
 * @brief 
 */


namespace common
{
namespace core
{
namespace io
{

/**
 * @class SelectEventIO
 * @brief [TODO:description]
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

	private:
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

} // !common
} // !core
} // !io

#endif // !COMMON_SELECTEVENTIO_HPP
