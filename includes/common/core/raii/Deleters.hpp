/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Deleters.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_DELETERS_HPP
#define COMMON_DELETERS_HPP

/**
 * @file Deleters.hpp
 * @brief Default deleter functors for use with smart pointers.
 */

#include <netdb.h>

namespace common
{
namespace core
{
namespace raii
{

/**
 * @brief Interface for custom deleter functors.
 *
 * Provides a virtual destructor and a pure virtual destroy method
 * for deleting objects via a void pointer. Intended for use with
 * smart pointer implementations that require custom deletion logic.
 */
struct IDeleter
{
	virtual ~IDeleter() {}

	virtual void destroy(void *ptr) = 0;
};

/**
 * @brief Default deleter for single objects.
 *
 * Deletes an object of type T using the delete operator.
 * Intended for use with smart pointers managing single objects.
 *
 * @tparam T Type of the object to delete.
 */
template<typename T>
struct DefaultDelete : public IDeleter
{
	void	destroy(void *ptr) throw() { delete static_cast<T *>(ptr); }
    void	operator()(T *ptr) const { delete ptr; }
};

/**
 * @brief Default deleter for arrays.
 *
 * Deletes an array of type T using the delete[] operator.
 * Intended for use with smart pointers managing arrays.
 *
 * @tparam T Type of the array elements to delete.
 */
template<typename T>
struct DefaultDelete<T[]> : public IDeleter
{
	void	destroy(void *ptr) throw() { delete[] static_cast<T *>(ptr); }
    void	operator()(T *ptr) const { delete[] ptr; }
};

/**
 * @brief Deleter for addrinfo structures.
 *
 * Provides custom deletion logic for objects of type struct addrinfo,
 * using freeaddrinfo. Intended for use with smart pointers managing
 * addrinfo pointers returned by getaddrinfo.
 */
struct AddrinfoDeleter : public IDeleter
{
	void	destroy(void *ptr) throw() { freeaddrinfo(static_cast<struct addrinfo *>(ptr)); } 
    void	operator()(struct addrinfo *ptr) const { freeaddrinfo(ptr); }
};

} // !raii
} // !core
} // !common
#endif // !COMMON_DELETERS_HPP

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
