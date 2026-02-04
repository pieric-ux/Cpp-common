/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventFactoryIO.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/02/03                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common/core/io/SelectEventIO.hpp>
#include <common/core/io/PollEventIO.hpp>
#include <common/core/raii/UniquePtr.hpp>
#include <common/core/io/EventFactoryIO.hpp>
#include <stdexcept>

/**
 * @file EventFactoryIO.cpp
 * @brief Implementation of the IEventIO factory for creating event handler instances.
 */


namespace common
{
namespace core
{
namespace io
{

/**
 * @brief Private constructor to prevent instantiation.
 *
 * This class being a static factory, it should not be instantiated.
 */
EventFactoryIO::EventFactoryIO() {}

/**
 * @brief Private destructor.
 */
EventFactoryIO::~EventFactoryIO() {}

/**
 * @brief Creates an IEventIO instance according to the specified type.
 *
 * This method returns a raw pointer because in C++98, without move semantics,
 * it is impossible to return a UniquePtr with explicit constructor.
 * The caller must manage memory, ideally by wrapping the result
 * in a UniquePtr immediately after the call.
 *
 * @param type Implementation type ("select" or "poll").
 * @return Raw pointer to the created instance, or NULL on error.
 */
IEventIO* EventFactoryIO::create(const std::string &type)
{
	switch (stringToType(type)) 
	{
		case SELECT:
			return new SelectEventIO();
		case POLL:
			return new PollEventIO();
	}
	return NULL;
}

/**
 * @brief Converts a string to internal enumeration type.
 *
 * @param type String representing the implementation type.
 * @return Corresponding enumeration type.
 * @throw std::runtime_error If the provided type is not recognized.
 */
EventFactoryIO::e_Type EventFactoryIO::stringToType(const std::string type)
{
	if (type == "select")
		return (SELECT);
	if (type == "poll")
		return (POLL);
	throw std::runtime_error("EventFactoryIO: unknown type");
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
