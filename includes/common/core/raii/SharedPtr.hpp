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

#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

/**
 * @file SharedPtr.hpp
 * @brief Implementation of a reference-counted shared ownership smart pointer 
 * with customizable deleter.
 */

#include "common/core/utils/algoUtils.hpp"
#include "common/core/raii/Deleters.hpp"
#include <cstddef>

namespace common
{
namespace core
{
namespace raii
{

/**
 * @def MAKE_SHARED(TYPE, ...)
 * @brief Helper macro to create a SharedPtr for a single object.
 *
 * Constructs a SharedPtr of the specified TYPE, forwarding any constructor arguments.
 */
#define MAKE_SHARED(TYPE, ...) SharedPtr<TYPE>(new TYPE(__VA_ARGS__))

/**
 * @def MAKE_SHARED_ARRAY(TYPE, SIZE)
 * @brief Helper macro to create a SharedPtr for an array.
 *
 * Constructs a SharedPtr for an array of TYPE with the given SIZE.
 */
#define MAKE_SHARED_ARRAY(TYPE, SIZE) SharedPtr<TYPE[]>(new TYPE[SIZE])

/**
 * @class SharedPtrBase
 * @brief Base class for reference-counted shared ownership smart pointers.
 *
 * Manages a pointer, a deleter, and a reference count for shared ownership.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 */
template<typename T, typename Deleter>
class SharedPtrBase
{
	public:
		void			reset(T *ptr = 0) throw();
		void			swap(SharedPtrBase &other) throw();

		T				*get() const throw();
		Deleter			&getDeleter() throw();
		const Deleter	&getDeleter() const throw();

		std::size_t		useCount() const throw();
		bool			unique() const throw();

		operator bool() const throw();

	protected:
		T				*_ptr;
		Deleter			_deleter;
		std::size_t		*_count;

		explicit SharedPtrBase(T *ptr = 0, Deleter deleter = Deleter()) throw();
		~SharedPtrBase();

		SharedPtrBase(const SharedPtrBase &rhs) throw();
		SharedPtrBase &operator=(const SharedPtrBase &rhs) throw();
};

/**
 * @class SharedPtr
 * @brief Reference-counted shared ownership smart pointer for single objects.
 *
 * Provides shared ownership semantics for a dynamically allocated object.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor (default: DefaultDelete<T>).
 */
template<typename T, typename Deleter = DefaultDelete<T> >
class SharedPtr : public SharedPtrBase<T, Deleter>
{
	public:
		typedef SharedPtrBase<T, Deleter> Base;

		explicit SharedPtr(T *ptr = 0, Deleter deleter = Deleter()) throw() : Base(ptr, deleter) {}
		~SharedPtr() {}

		SharedPtr(const SharedPtr &rhs) throw() : Base(rhs) {}
		SharedPtr &operator=(const SharedPtr &rhs) throw()
		{
			Base::operator=(rhs);
			return (*this);
		}

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
 * @tparam Deleter Type of the deleter functor.
 */
template<typename T, typename Deleter>
class SharedPtr<T[], Deleter> : SharedPtrBase<T, Deleter>
{
	public:
		typedef SharedPtrBase<T, Deleter> Base;

		explicit SharedPtr(T *ptr = 0, Deleter deleter = Deleter()) throw() : Base(ptr, deleter) {}
		~SharedPtr() {}

		SharedPtr(const SharedPtr &rhs) throw() : Base(rhs) {}
		SharedPtr &operator=(const SharedPtr &rhs) throw()
		{
			Base::operator=(rhs);
			return (*this);
		}

		T	&operator[](std::size_t i) const;
};

/**
 * @brief Constructs a SharedPtrBase with a pointer and deleter.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param ptr The pointer to manage.
 * @param deleter The deleter to use.
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::SharedPtrBase(T *ptr, Deleter deleter) throw() : _ptr(ptr), _deleter(deleter), _count(0)
{
	if (this->_ptr)
		this->_count = new std::size_t(1);
}

/**
 * @brief Destructor. Decrements reference count and deletes managed pointer if needed.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::~SharedPtrBase()
{
	reset();
}

/**
 * @brief Copy constructor. Increments reference count.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param rhs SharedPtrBase to copy from.
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::SharedPtrBase(const SharedPtrBase &rhs) throw() : _ptr(rhs._ptr), _deleter(rhs._deleter), _count(rhs._count)
{
	if (this->_count)
		++(this->_count);
}

/**
 * @brief Assignment operator. Handles reference count and pointer management.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param rhs SharedPtrBase to assign from.
 * @return Reference to this instance.
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>	&SharedPtrBase<T, Deleter>::operator=(const SharedPtrBase &rhs) throw()
{
	if (this != &rhs)
	{
		this->reset();
		this->_ptr = rhs._ptr;
		this->_deleter = rhs._deleter;
		this->_count = rhs._count;
		if (this->_count)
			++(*this->_count);
	}
	return (*this);
}

/**
 * @brief Replaces the managed pointer with a new one, updating reference count and deleting if necessary.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param ptr The new pointer to manage.
 */
template<typename T, typename Deleter>
void	SharedPtrBase<T, Deleter>::reset(T *ptr) throw()
{
	if (this->_ptr == ptr)
		return ;
	if (this->_count && --(*this->_count) == 0)
	{
		this->_deleter(this->_ptr);
		delete this->_count;
	}

	if (ptr)
	{
		this->_ptr = ptr;
		this->_count = new std::size_t(1);
	}
	else
	{
		this->_ptr = 0;
		this->_count = 0;
	}
}

/**
 * @brief Swaps the managed pointer, deleter, and reference count with another SharedPtrBase.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param other The other SharedPtrBase to swap with.
 */
template<typename T, typename Deleter>
void	SharedPtrBase<T, Deleter>::swap(SharedPtrBase &other) throw()
{
	utils::swap(this->_ptr, other._ptr);
	utils::swap(this->_deleter, other._deleter);
	utils::swap(this->_count, other._count);
}

/**
 * @brief Gets the managed pointer.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return The managed pointer.
 */
template<typename T, typename Deleter>
T	*SharedPtrBase<T, Deleter>::get() const throw()
{
	return (this->_ptr);
}

/**
 * @brief Gets a reference to the deleter.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Reference to the deleter.
 */
template<typename T, typename Deleter>
Deleter	&SharedPtrBase<T, Deleter>::getDeleter() throw()
{
	return (this->_deleter);
}

/**
 * @brief Gets a const reference to the deleter.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Const reference to the deleter.
 */
template<typename T, typename Deleter>
const Deleter	&SharedPtrBase<T, Deleter>::getDeleter() const throw()
{
	return (this->_deleter);
}

/**
 * @brief Gets the number of SharedPtr instances sharing ownership.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Reference count.
 */
template<typename T, typename Deleter>
std::size_t		SharedPtrBase<T, Deleter>::useCount() const throw()
{
	return (this->_count ? *this->_count : 0);
}

/**
 * @brief Checks if this is the only SharedPtr owning the managed pointer.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return True if unique, false otherwise.
 */
template<typename T, typename Deleter>
bool	SharedPtrBase<T, Deleter>::unique() const throw()
{
	return (this->useCount() == 1);
}

/**
 * @brief Conversion operator to bool for SharedPtrBase.
 *
 * Allows checking if the SharedPtrBase currently manages a non-null pointer.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return true if the managed pointer is not null, false otherwise.
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::operator bool() const throw()
{
	return (this->_ptr != 0);
}

/**
 * @brief Member access operator for SharedPtr.
 * 
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Pointer to the managed object.
 */
template<typename T, typename Deleter>
T	*SharedPtr<T, Deleter>::operator->() const throw()
{
	return (this->_ptr);
}

/**
 * @brief Dereference operator for SharedPtr.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Reference to the managed object.
 */
template<typename T, typename Deleter>
T	&SharedPtr<T, Deleter>::operator*() const throw()
{
	return (*this->_ptr);
}

/**
 * @brief Array subscript operator for SharedPtr<T[], Deleter>.
 *
 * @tparam T Type of the managed array elements.
 * @tparam Deleter Type of the deleter functor.
 * @param i Index of the element.
 * @return Reference to the element at index i.
 */
template<typename T, typename Deleter>
T	&SharedPtr<T[], Deleter>::operator[](std::size_t i) const
{
	return (this->_ptr[i]);
}

/**
 * @brief Equality comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for equality.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam TDeleter Deleter type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @tparam UDeleter Deleter type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if both SharedPtr instances manage the same pointer, false otherwise.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator==(const SharedPtr<T, TDeleter> &lhs, const SharedPtr<U, UDeleter> &rhs) throw()
{
	return (lhs.get() == rhs.get());
}

/**
 * @brief Inequality comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for inequality.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam TDeleter Deleter type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @tparam UDeleter Deleter type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if both SharedPtr instances manage different pointers, false otherwise.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator!=(const SharedPtr<T, TDeleter> &lhs, const SharedPtr<U, UDeleter> &rhs) throw()
{
	return (!(lhs == rhs));
}

/**
 * @brief Less-than comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam TDeleter Deleter type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @tparam UDeleter Deleter type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is less than rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator<(const SharedPtr<T, TDeleter> &lhs, const SharedPtr<U, UDeleter> &rhs)
{
	return (lhs.get() < rhs.get());
}

/**
 * @brief Greater-than comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam TDeleter Deleter type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @tparam UDeleter Deleter type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is greater than rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator>(const SharedPtr<T, TDeleter> &lhs, const SharedPtr<U, UDeleter> &rhs)
{
	return (rhs < lhs);
}

/**
 * @brief Less-than-or-equal comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam TDeleter Deleter type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @tparam UDeleter Deleter type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is less than or equal to rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator<=(const SharedPtr<T, TDeleter> &lhs, const SharedPtr<U, UDeleter> &rhs)
{
	return (!(lhs > rhs));
}

/**
 * @brief Greater-than-or-equal comparison operator for SharedPtr.
 *
 * Compares the managed pointers of two SharedPtr instances for ordering.
 *
 * @tparam T Type of the first SharedPtr.
 * @tparam TDeleter Deleter type of the first SharedPtr.
 * @tparam U Type of the second SharedPtr.
 * @tparam UDeleter Deleter type of the second SharedPtr.
 * @param lhs Left-hand side SharedPtr.
 * @param rhs Right-hand side SharedPtr.
 * @return true if lhs's managed pointer is greater than or equal to rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator>=(const SharedPtr<T, TDeleter> &lhs, const SharedPtr<U, UDeleter> &rhs)
{
	return (!(lhs < rhs));
}

/**
 * @brief Equality comparison operator for SharedPtr and raw pointer.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for equality.
 *
 * @tparam T Type of the SharedPtr.
 * @tparam Deleter Deleter type of the SharedPtr.
 * @param lhs SharedPtr instance.
 * @param null_ptr Raw pointer to compare.
 * @return true if the SharedPtr manages the same pointer as null_ptr.
 */
template<typename T, typename Deleter>
bool operator==(const SharedPtr<T, Deleter> &lhs, const T *null_ptr) throw()
{
	return (lhs.get() == null_ptr);
}

/**
 * @brief Equality comparison operator for raw pointer and SharedPtr.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for equality.
 *
 * @tparam T Type of the SharedPtr.
 * @tparam Deleter Deleter type of the SharedPtr.
 * @param null_ptr Raw pointer to compare.
 * @param rhs SharedPtr instance.
 * @return true if the SharedPtr manages the same pointer as null_ptr.
 */
template<typename T, typename Deleter>
bool operator==(const T *null_ptr, const SharedPtr<T, Deleter> &rhs) throw()
{
	return (rhs.get() == null_ptr);
}

/**
 * @brief Inequality comparison operator for SharedPtr and raw pointer.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for inequality.
 *
 * @tparam T Type of the SharedPtr.
 * @tparam Deleter Deleter type of the SharedPtr.
 * @param lhs SharedPtr instance.
 * @param null_ptr Raw pointer to compare.
 * @return true if the SharedPtr manages a different pointer than null_ptr.
 */
template<typename T, typename Deleter>
bool operator!=(const SharedPtr<T, Deleter> &lhs, const T *null_ptr) throw()
{
	return (!(lhs == null_ptr));
}

/**
 * @brief Inequality comparison operator for raw pointer and SharedPtr.
 *
 * Compares the managed pointer of a SharedPtr with a raw pointer for inequality.
 *
 * @tparam T Type of the SharedPtr.
 * @tparam Deleter Deleter type of the SharedPtr.
 * @param null_ptr Raw pointer to compare.
 * @param rhs SharedPtr instance.
 * @return true if the SharedPtr manages a different pointer than null_ptr.
 */
template<typename T, typename Deleter>
bool operator!=(const T *null_ptr, const SharedPtr<T, Deleter> &rhs) throw()
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
	if (ptr)
	{
		result._ptr = ptr;
		result._count = rhs._count;
		result._deleter = rhs._deleter;
		if (result._count)
			++(*result._count);
	}
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
	if (ptr)
	{
		result._ptr = ptr;
		result._count = rhs._count;
		result._deleter = rhs._deleter;
		if (result._count)
			++(*result._count);
	}
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
	if (ptr)
	{
		result._ptr = ptr;
		result._count = rhs._count;
		result._deleter = rhs._deleter;
		if (result._count)
			++(*result._count);
	}
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
	if (ptr)
	{
		result._ptr = ptr;
		result._count = rhs._count;
		result._deleter = rhs._deleter;
		if (result._count)
			++(*result._count);
	}
	return (result);
}

} // !raii
} // !core
} // !common

#endif // !SHAREDPTR_HPP
