/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeakPtr.hpp                                      :+:      :+:    :+:	  */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/11/05                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WEAKPTR_HPP
#define WEAKPTR_HPP

/**
 * @file WeakPtr.hpp
 * @brief Implements a weak reference to an object managed by SharedPtr.
 */

#include "common/core/utils/algoUtils.hpp"
#include "common/core/raii/SharedPtr.hpp"

namespace common
{
namespace core
{
namespace raii
{

template<typename T>

/**
 * @class WeakPtr
 * @brief Non-owning weak reference to an object managed by SharedPtr.
 *
 * `WeakPtr<T>` observes the control block used by `SharedPtr<T>` without
 * contributing to the shared ownership count. Use `lock()` to attempt to
 * obtain a `SharedPtr<T>` if the managed object still exists.
 *
 * @tparam T Type of the managed object.
 */
class WeakPtr
{
	public:
		explicit WeakPtr(const SharedPtr<T> &sp = SharedPtr<T>()) throw();
		~WeakPtr();

		WeakPtr(const WeakPtr &other) throw();
		WeakPtr &operator=(const WeakPtr &other) throw();
		
		void reset() throw();
		void swap(WeakPtr &other) throw();

		bool expired() const throw();
		SharedPtr<T> lock() const throw();

	private:
		void release() throw();

		SharedPtrControlBlock *_cb;
};

/**
 * @brief Construct a WeakPtr observing the same control block as sp.
 *
 * If `sp` is non-empty this increments the `weak_count` in the control block
 * so that the block remains alive until all weak references are released.
 * @tparam T Type of the managed object.
 * @param sp SharedPtr to observe (defaults to empty SharedPtr).
 */
template<typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T> &sp) throw() : _cb(sp._cb)
{ 
	if (_cb)
		++_cb->weak_count;
}

/**
 * @brief Destructor.
 *
 * Releases the weak reference (decrements `weak_count`) and deletes the
 * control block if both shared and weak counts reach zero.
 */
template<typename T>
WeakPtr<T>::~WeakPtr()
{ 
	release();
}

/**
 * @brief Copy constructor: shares observation of the same control block.
 *
 * Increments `weak_count` on the control block.
 * @param other WeakPtr to copy from.
 */
template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr &other) throw() : _cb(other._cb)
{
	if (_cb)
		++_cb->weak_count;
}

/**
 * @brief Copy assignment operator.
 *
 * Releases current observation and then observes the control block of
 * `other`, incrementing its `weak_count`.
 * @param other WeakPtr to assign from.
 * @return Reference to *this.
 */
template<typename T>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr<T> &other) throw()
{
	if (this != &other)
	{
		release();
		_cb = other._cb;
		if (_cb)
			++_cb->weak_count;
	}
	return (*this);
}

/**
 * @brief Reset the WeakPtr to empty.
 *
 * Releases the weak reference and clears the internal control block pointer.
 */
template<typename T>
void WeakPtr<T>::reset() throw() 
{ 
	release();
}

/**
 * @brief Swap observations with another WeakPtr.
 *
 * Exchanges the internal control block pointer with `other` using
 * `common::core::utils::swap`.
 * @param other WeakPtr to swap with.
 */
template<typename T>
void WeakPtr<T>::swap(WeakPtr<T> &other) throw() 
{ 
	utils::swap(_cb, other._cb);
}

/**
 * @brief Check whether the managed object has been destroyed.
 *
 * Returns true if there is no control block or if the shared count has
 * dropped to zero (the managed object was destroyed).
 * @return true if the observed object is expired, false otherwise.
 */
template<typename T>
bool WeakPtr<T>::expired() const throw()
{
	return (!_cb || _cb->count == 0);
}

/**
 * @brief Create a SharedPtr that shares ownership of the managed object.
 *
 * If the managed object still exists (`count > 0`) a `SharedPtr<T>` is
 * returned that increments the shared count; otherwise an empty
 * `SharedPtr<T>` is returned.
 * @return SharedPtr<T> owning the object if still alive, empty otherwise.
 */
template<typename T>
SharedPtr<T> WeakPtr<T>::lock() const throw()
{
	SharedPtr<T> result;
	if (_cb && _cb->count > 0) {
		result._cb = _cb;
		++result._cb->count;
	}
	else
		result._cb = 0;
	return (result);
}

/**
 * @brief Internal helper that releases the weak reference.
 *
 * Decrements `weak_count` and deletes the control block if both
 * `weak_count` and `count` reach zero. This is a private implementation
 * detail used by the destructor, assignment operator and reset().
 */
template<typename T>
void WeakPtr<T>::release() throw()
{
	if (!_cb)
		return;
	if (--_cb->weak_count == 0 && _cb->count == 0)
		delete _cb;
	_cb = 0;
}

} // !raii
} // !core
} // !common

#endif // !WEAKPTR_HPP
