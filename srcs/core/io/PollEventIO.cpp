/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollEventIO.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/01/30 17:40:31 by blucken          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common/core/io/IEventIO.hpp"
#include <algorithm>
#include <common/core/io/PollEventIO.hpp>

namespace common
{
namespace core
{
namespace io
{
PollEventIO::PollEventIO() {}

PollEventIO::~PollEventIO() {}

int	wait(int timeout_ms)
{

}

void add(int fd, e_Event mask)
{

}

void remove(int fd)
{

}

void update(int fd, e_Event mask)
{

}

e_Event getEvent(int fd) const
{

}

} //!io
} //!core
} //!common