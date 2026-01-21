/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedPtr.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/23                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common/common.hpp>

/**
 * @file SharedPtr.cpp
 * @brief Implementation of a reference-counted shared ownership smart pointer 
 * with customizable deleter.
 */

namespace common
{
namespace core
{
namespace raii
{

/**
 * @brief Constructs a SharedPtrBase with a pointer and deleter.
 *
 * Initializes the control block if ptr is not null.
 * @param ptr The pointer to manage.
 * @param deleter Deleter to use for object destruction.
 */
SharedPtrBase::SharedPtrBase(void *ptr, IDeleter *deleter) throw() : _cb(0)
{
	if (ptr)
		this->_cb = new SharedPtrControlBlock(ptr, deleter);
}

/**
 * @brief Destructor for SharedPtrBase.
 *
 * Releases ownership and deletes managed object if reference count reaches zero.
 */
SharedPtrBase::~SharedPtrBase()
{
	reset();
}

/**
 * @brief Copy constructor for SharedPtrBase.
 *
 * Shares ownership and increments reference count.
 * @param rhs SharedPtrBase to copy from.
 */
SharedPtrBase::SharedPtrBase(const SharedPtrBase &rhs) throw() : _cb(rhs._cb) 
{
	if (this->_cb)
		++this->_cb->count;
}

/**
 * @brief Assignment operator. Handles reference count and pointer management.
 *
 * @tparam T Type of the managed object.
 * @param rhs SharedPtrBase to assign from.
 * @return Reference to this instance.
 */
SharedPtrBase	&SharedPtrBase::operator=(const SharedPtrBase &rhs) throw()
{
	if (this != &rhs)
	{
		this->reset();
		this->_cb = rhs._cb;
		if (this->_cb)
			++this->_cb->count;
	}
	return (*this);
}

/**
 * @brief Reset the managed pointer.
 * 
 * Decrements reference count and deletes managed object if needed.
 * If ptr is provided, takes ownership of new pointer.
 * @param ptr New pointer to manage (default: nullptr).
 */
void	SharedPtrBase::reset(void *ptr) throw()
{
	if (this->_cb && --this->_cb->count == 0)
	{
		if (this->_cb->ptr && this->_cb->deleter)
		{
			this->_cb->deleter->destroy(this->_cb->ptr);
			delete this->_cb->deleter;
		}
		if (this->_cb->weak_count == 0)
			delete this->_cb;
	}
	this->_cb = 0;
	if (ptr)
		this->_cb = new SharedPtrControlBlock(ptr, 0);
}

/**
 * @brief Swap control blocks with another SharedPtrBase.
 *
 * Exchanges ownership and reference count with other.
 * @param other The other SharedPtrBase to swap with.
 */
void	SharedPtrBase::swap(SharedPtrBase &other) throw()
{
	utils::swap(this->_cb, other._cb);
}

/**
 * @brief Gets the number of SharedPtr instances sharing ownership.
 *
 * @return Reference count.
 */
std::size_t		SharedPtrBase::useCount() const throw()
{
	return (this->_cb ? this->_cb->count : 0);
}

/**
 * @brief Checks if this is the only SharedPtr owning the managed pointer.
 *
 * @return True if unique, false otherwise.
 */
bool	SharedPtrBase::unique() const throw()
{
	return (this->useCount() == 1);
}

/**
 * @brief Conversion operator to bool for SharedPtrBase.
 * 
 * Checks if the SharedPtrBase currently manages a non-null pointer.
 * @return true if the managed pointer is not null, false otherwise.
 */
SharedPtrBase::operator bool() const throw()
{
	return (this->_cb && this->_cb->ptr != 0);
}

} // !raii
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
