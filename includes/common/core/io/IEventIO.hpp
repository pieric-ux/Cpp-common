/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/01/26                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_IEVENTIO_HPP
#define COMMON_IEVENTIO_HPP

/**
 * @file IEventIO.hpp
 * @brief 
 */

namespace common
{
namespace core
{
namespace io
{

/**
 * @class IEventIO
 * @brief [TODO:description]
 */
class IEventIO
{
	public:
		enum e_Event
		{
			E_IN = 1 << 0,
			E_OUT = 1 << 1,
			E_EXCEPT = 1 << 2,
		};

		virtual ~IEventIO() {};

		virtual int	wait(int timeout_ms) = 0;
		virtual void add(int fd, e_Event mask) = 0;
		virtual void remove(int fd) = 0;
		virtual void update(int fd, e_Event mask) = 0;
		virtual void clear() = 0;
};

} // !io
} // !core
} // !common

#endif // !COMMON_IEVENTIO_HPP

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
