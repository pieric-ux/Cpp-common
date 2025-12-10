/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedPtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_SHAREDPTR_HPP
#define COMMON_SHAREDPTR_HPP

/**
 * @file SharedPtr.hpp
 * @brief Reference-counted shared ownership smart pointer implementation.
 */

#include <common/core/raii/Deleters.hpp>
#include <cstddef>
#include <cstdio>

namespace common
{
namespace core
{
namespace raii
{

/**
 * @struct SharedPtrControlBlock
 * @brief Internal control block for SharedPtr reference counting.
 *
 * Holds the managed pointer, deleter, and reference count.
 */
struct SharedPtrControlBlock 
{
	void			*ptr;
	IDeleter		*deleter;
	std::size_t		count;
	std::size_t		weak_count;

	SharedPtrControlBlock(void *p, IDeleter *d) : ptr(p), deleter(d), count(1), weak_count(0) {}
};

/**
 * @def MAKE_SHARED(TYPE, ...)
 * @brief Helper macro to create a SharedPtr for a single object.
 *
 * Constructs a SharedPtr of the specified TYPE, forwarding any constructor arguments.
 */
#define MAKE_SHARED(TYPE, ...) common::core::raii::SharedPtr<TYPE>(new TYPE(__VA_ARGS__))

/**
 * @def MAKE_SHARED_ARRAY(TYPE, SIZE)
 * @brief Helper macro to create a SharedPtr for an array.
 *
 * Constructs a SharedPtr for an array of TYPE with the given SIZE.
 */
#define MAKE_SHARED_ARRAY(TYPE, SIZE) common::core::raii::SharedPtr<TYPE[]>(new TYPE[SIZE])

template<typename T> class SharedPtr;

/**
 * @class SharedPtrBase
 * @brief Base class for reference-counted shared ownership smart pointers.
 *
 * Manages a pointer, a deleter, and a reference count for shared ownership.
 */
class SharedPtrBase
{
	public:
		std::size_t		useCount() const throw();
		bool			unique() const throw();

		operator bool() const throw();

	protected:
		SharedPtrControlBlock *_cb;

		explicit SharedPtrBase(void *ptr = 0, IDeleter *deleter = 0) throw();
		~SharedPtrBase();

		SharedPtrBase(const SharedPtrBase &rhs) throw();
		
		SharedPtrBase &operator=(const SharedPtrBase &rhs) throw();
		
		void	reset(void *ptr = 0) throw();
		void	swap(SharedPtrBase &other) throw();


		template<typename X>
		friend class WeakPtr;

		template<typename X, typename Y>
		friend SharedPtr<X> staticPointerCast(const SharedPtr<Y> &rhs) throw();

		template<typename X, typename Y>
		friend SharedPtr<X> dynamicPointerCast(const SharedPtr<Y> &rhs) throw();

		template<typename X, typename Y>
		friend SharedPtr<X> constPointerCast(const SharedPtr<Y> &rhs) throw();

		template<typename X, typename Y>
		friend SharedPtr<X> reinterpretPointerCast(const SharedPtr<Y> &rhs) throw();
};

/**
 * @class SharedPtr
 * @brief Reference-counted shared ownership smart pointer for single objects.
 *
 * Provides shared ownership semantics for a dynamically allocated object.
 *
 * @tparam T Type of the managed object.
 */
template<typename T>
class SharedPtr : public SharedPtrBase
{
	public:
		explicit SharedPtr(T *ptr = 0) throw() : SharedPtrBase(ptr, ptr ? static_cast<IDeleter *>(new DefaultDelete<T>()) : 0) {}
		~SharedPtr() {}

		SharedPtr(const SharedPtr &rhs) throw() : SharedPtrBase(rhs) {}
		SharedPtr &operator=(const SharedPtr &rhs) throw()
		{
			SharedPtrBase::operator=(rhs);
			return (*this);
		}
		T	*get() const throw();
		T	*operator->() const throw();
		T	&operator*() const throw();
};

/**
 * @class SharedPtr<T[], Deleter>
 * @brief Reference-counted shared ownership smart pointer specialization for arrays.
 *
 * Provides shared ownership semantics for a dynamically allocated array.
 *
 * @tparam T Type of the managed array elements.
 */
template<typename T>
class SharedPtr<T[]> : public SharedPtrBase
{
	public:
		explicit SharedPtr(T *ptr = 0) throw() : SharedPtrBase(ptr, ptr ? static_cast<IDeleter *>(new DefaultDelete<T[]>()) : 0) {}
		~SharedPtr() {}

		SharedPtr(const SharedPtr &rhs) throw() : SharedPtrBase(rhs) {}
		SharedPtr &operator=(const SharedPtr &rhs) throw()
		{
			SharedPtrBase::operator=(rhs);
			return (*this);
		}

		T	&operator[](std::size_t i) const;
};

/**
 * @brief Get the raw pointer to the managed object.
 *
 * @tparam T Type of the managed object.
 * @return Pointer to the managed object.
 */
template<typename T>
T	*SharedPtr<T>::get() const throw()
{
	return (static_cast<T *>(this->_cb ? this->_cb->ptr : 0));
}

/**
 * @brief Member access operator for SharedPtr.
 * 
 * @tparam T Type of the managed object.
 * @return Pointer to the managed object.
 */
template<typename T>
T	*SharedPtr<T>::operator->() const throw()
{
	return (this->get());
}

/**
 * @brief Dereference operator for SharedPtr.
 *
 * @tparam T Type of the managed object.
 * @return Reference to the managed object.
 */
template<typename T>
T	&SharedPtr<T>::operator*() const throw()
{
	return (*this->get());
}

/**
 * @brief Array subscript operator for SharedPtr<T[], Deleter>.
 *
 * @tparam T Type of the managed array elements.
 * @param i Index of the element.
 * @return Reference to the element at index i.
 */
template<typename T>
T	&SharedPtr<T[]>::operator[](std::size_t i) const
{
	return (static_cast<T *>(this->_cb->ptr)[i]);
}

/**
 * @brief Equality comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for equality.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if both SharedPtr instances manage the same pointer, false otherwise.
 */
template<typename T, typename U>
bool	operator==(const SharedPtr<T> &lhs, const SharedPtr<U> &rhs) throw()
{
	return (lhs.get() == rhs.get());
}

/**
 * @brief Inequality comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for inequality.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if both SharedPtr instances manage different pointers, false otherwise.
 */
template<typename T, typename U>
bool	operator!=(const SharedPtr<T> &lhs, const SharedPtr<U> &rhs) throw()
{
	return (!(lhs == rhs));
}

/**
 * @brief Less-than comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is less than rhs's managed pointer.
 */
template<typename T, typename U>
bool	operator<(const SharedPtr<T> &lhs, const SharedPtr<U> &rhs)
{
	return (lhs.get() < rhs.get());
}

/**
 * @brief Greater-than comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is greater than rhs's managed pointer.
 */
template<typename T, typename U>
bool	operator>(const SharedPtr<T> &lhs, const SharedPtr<U> &rhs)
{
	return (rhs < lhs);
}

/**
 * @brief Less-than-or-equal comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is less than or equal to rhs's managed pointer.
 */
template<typename T, typename U>
bool	operator<=(const SharedPtr<T> &lhs, const SharedPtr<U> &rhs)
{
	return (!(lhs > rhs));
}

/**
 * @brief Greater-than-or-equal comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is greater than or equal to rhs's managed pointer.
 */
template<typename T, typename U>
bool	operator>=(const SharedPtr<T> &lhs, const SharedPtr<U> &rhs)
{
	return (!(lhs < rhs));
}

/**
 * @brief Equality comparison operator for SharedPtr and raw pointer.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for equality.
 *
 * @tparam T Type of the SharedPtr.
 * @param lhs SharedPtr instance.
 * @param null_ptr Raw pointer to compare.
 * @return true if the SharedPtr manages the same pointer as null_ptr.
 */
template<typename T>
bool operator==(const SharedPtr<T> &lhs, const T *null_ptr) throw()
{
	return (lhs.get() == null_ptr);
}

/**
 * @brief Equality comparison operator for raw pointer and SharedPtr.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for equality.
 *
 * @tparam T Type of the SharedPtr.
 * @param null_ptr Raw pointer to compare.
 * @param rhs SharedPtr instance.
 * @return true if the SharedPtr manages the same pointer as null_ptr.
 */
template<typename T>
bool operator==(const T *null_ptr, const SharedPtr<T> &rhs) throw()
{
	return (rhs.get() == null_ptr);
}

/**
 * @brief Inequality comparison operator for SharedPtr and raw pointer.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for inequality.
 *
 * @tparam T Type of the SharedPtr.
 * @param lhs SharedPtr instance.
 * @param null_ptr Raw pointer to compare.
 * @return true if the SharedPtr manages a different pointer than null_ptr.
 */
template<typename T>
bool operator!=(const SharedPtr<T> &lhs, const T *null_ptr) throw()
{
	return (!(lhs == null_ptr));
}

/**
 * @brief Inequality comparison operator for raw pointer and SharedPtr.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for inequality.
 *
 * @tparam T Type of the SharedPtr.
 * @param null_ptr Raw pointer to compare.
 * @param rhs SharedPtr instance.
 * @return true if the SharedPtr manages a different pointer than null_ptr.
 */
template<typename T>
bool operator!=(const T *null_ptr, const SharedPtr<T> &rhs) throw()
{
	return (!(rhs.get() == null_ptr));
}

/**
 * @brief Casts a SharedPtr<U> to SharedPtr<T> using static_cast.
 *
 * @tparam T Target type for the cast.
 * @tparam U Source type of the SharedPtr.
 * @param rhs SharedPtr<U> to cast.
 * @return SharedPtr<T> pointing to the same object if cast is valid, otherwise an empty SharedPtr<T>.
 */
template<typename T, typename U>
SharedPtr<T> staticPointerCast(const SharedPtr<U> &rhs) throw()
{
    T *ptr = static_cast<T *>(rhs.get());
	raii::SharedPtr<T> result;
	result._cb = rhs._cb;
	if (ptr && result._cb)
		++result._cb->count;
	else
		result._cb = 0;
	return (result);
}

/**
 * @brief Casts a SharedPtr<U> to SharedPtr<T> using dynamic_cast.
 *
 * @tparam T Target type for the cast.
 * @tparam U Source type of the SharedPtr.
 * @param rhs SharedPtr<U> to cast.
 * @return SharedPtr<T> pointing to the same object if cast is valid, otherwise an empty SharedPtr<T>.
 */
template<typename T, typename U>
SharedPtr<T> dynamicPointerCast(const SharedPtr<U> &rhs) throw()
{
	T *ptr = dynamic_cast<T *>(rhs.get());
	raii::SharedPtr<T> result;
	if (ptr && rhs._cb)
	{
		result._cb = rhs._cb;
		++result._cb->count;
	}
	else
		result._cb = 0;
	return (result);
}

/**
 * @brief Casts a SharedPtr<U> to SharedPtr<T> using const_cast.
 *
 * @tparam T Target type for the cast.
 * @tparam U Source type of the SharedPtr.
 * @param rhs SharedPtr<U> to cast.
 * @return SharedPtr<T> pointing to the same object if cast is valid, otherwise an empty SharedPtr<T>.
 */
template<typename T, typename U>
SharedPtr<T> constPointerCast(const SharedPtr<U> &rhs) throw()
{
	T *ptr = const_cast<T *>(rhs.get());
	raii::SharedPtr<T> result;
	result._cb = rhs._cb;
	if (ptr && result._cb)
		++result._cb->count;
	else
		result._cb = 0;
	return (result);
}

/**
 * @brief Casts a SharedPtr<U> to SharedPtr<T> using reinterpret_cast.
 *
 * @tparam T Target type for the cast.
 * @tparam U Source type of the SharedPtr.
 * @param rhs SharedPtr<U> to cast.
 * @return SharedPtr<T> pointing to the same object if cast is valid, otherwise an empty SharedPtr<T>.
 */
template<typename T, typename U>
SharedPtr<T> reinterpretPointerCast(const SharedPtr<U> &rhs) throw()
{
	T *ptr = reinterpret_cast<T *>(rhs.get());
	raii::SharedPtr<T> result;
	result._cb = rhs._cb;
	if (ptr && result._cb)
		++result._cb->count;
	else
		result._cb = 0;
	return (result);
}

} // !raii
} // !core
} // !common

#endif // !COMMON_SHAREDPTR_HPP
