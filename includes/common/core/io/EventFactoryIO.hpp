/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventFactoryIO.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2026/02/03                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_EVENTFACTORYIO_HPP
#define COMMON_EVENTFACTORYIO_HPP

/**
 * @file EventFactoryIO.hpp
 * @brief Factory for creating IEventIO instances based on different implementations.
 */

#include <common/core/io/IEventIO.hpp>
#include <common/core/raii/UniquePtr.hpp>
#include <string>

namespace common
{
namespace core
{
namespace io
{

/**
 * @class EventFactoryIO
 * @brief Factory for creating I/O event handlers.
 *
 * This factory allows dynamic creation of different IEventIO implementations
 * (select, poll) based on a type string. The returned pointer must be managed
 * by the caller, ideally via a UniquePtr.
 *
 * @note In C++98, the factory returns a raw pointer because UniquePtr with explicit
 *       constructor cannot be returned directly without move semantics.
 *
 * Usage:
 * @code
 * IEventIO* rawPtr = EventFactoryIO::create("select");
 * common::core::raii::UniquePtr<IEventIO> ptr(rawPtr);
 * @endcode
 *
 * @startuml
 * class "EventFactoryIO" as EventFactoryIO [[classcommon_1_1core_1_1io_1_1_event_factory_i_o.html]] {
		--
		+ {static} createEventIO(type : e_Type) : IEventIO*
		- EventFactoryIO()
	}
 * @enduml
 */
class EventFactoryIO
{
	public:
		static IEventIO* create(const std::string &type);

	private:
		/**
		 * @enum e_Type
		 * @brief Types of IEventIO implementations.
		 *
		 * @startuml
		 * enum "e_Type" as e_Type {
			SELECT
			POLL
		}
		 * @enduml
		 */
		enum e_Type {
			SELECT, ///< Implementation based on select(2)
			POLL,   ///< Implementation based on poll(2)
		};

		EventFactoryIO();
		~EventFactoryIO();
		EventFactoryIO(const EventFactoryIO &rhs);
		EventFactoryIO &operator=(const EventFactoryIO &rhs);

		static e_Type stringToType(const std::string type);
};

} // !io
} // !core
} // !common

#endif // COMMON_EVENTFACTORYIO_HPP

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
