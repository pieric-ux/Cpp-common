/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniquePtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP

/**
 * @file UniquePtr.hpp
 * @brief
 */

#include "common/core/utils/algoUtils.hpp"
#include "Deleters.hpp"
#include <cstddef>

namespace common
{
namespace core
{
namespace raii
{

/**
 * @def MAKE_UNIQUE(TYPE, ...)
 * @brief Helper macro to create a UniquePtr for a single object.
 *
 * Constructs a UniquePtr of the specified TYPE, forwarding any constructor arguments.
 */
#define MAKE_UNIQUE(TYPE, ...)			UniquePtr<TYPE>(new TYPE(__VA_ARGS__))

/**
 * @def MAKE_UNIQUE_ARRAY(TYPE, SIZE)
 * @brief Helper macro to create a UniquePtr for an array.
 *
 * Constructs a UniquePtr for an array of TYPE with the given SIZE.
 */
#define MAKE_UNIQUE_ARRAY(TYPE, SIZE)	UniquePtr<TYPE[]>(new TYPE[SIZE])

/**
 * @class UniquePtrBase
 * @brief Base class for unique ownership smart pointers.
 *
 * Manages a pointer and a deleter, providing core unique pointer operations.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 */
template<typename T, typename Deleter>
class UniquePtrBase
{
	public:
		T				*release() throw();
		void			reset(T *ptr = 0) throw();
		void			swap(UniquePtrBase &other) throw();

		T				*get() const throw();
		Deleter			&getDeleter() throw();
		const Deleter	&getDeleter() const throw();

	protected:
		T		*_ptr;
		Deleter _deleter;

		explicit UniquePtrBase(T *ptr = 0, Deleter deleter = Deleter()) throw();
		~UniquePtrBase();

	private:
		UniquePtrBase(const UniquePtrBase &rhs); 
		UniquePtrBase &operator=(const UniquePtrBase &rhs);
};

/**
 * @class UniquePtr
 * @brief Unique ownership smart pointer for single objects.
 *
 * Provides unique ownership semantics for a dynamically allocated object.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor (default: DefaultDelete<T>).
 */
template<typename T, typename Deleter = DefaultDelete<T> >
class UniquePtr : public UniquePtrBase<T, Deleter>
{
	public:
		typedef UniquePtrBase<T, Deleter> Base;

		explicit UniquePtr(T *ptr = 0, Deleter deleter = Deleter()) throw() : Base(ptr, deleter) {}
		~UniquePtr() {}

		T	*operator->() const throw();
		T	&operator*() const throw();

	private:
		UniquePtr(const UniquePtr &rhs); 
		UniquePtr &operator=(const UniquePtr &rhs);
};

/**
 * @class UniquePtr<T[], Deleter>
 * @brief Unique ownership smart pointer specialization for arrays.
 *
 * Provides unique ownership semantics for a dynamically allocated array.
 *
 * @tparam T Type of the managed array elements.
 * @tparam Deleter Type of the deleter functor.
 */
template<typename T, typename Deleter>
class UniquePtr<T[], Deleter> : public UniquePtrBase<T, Deleter>
{
	public:
		typedef UniquePtrBase<T, Deleter> Base;

		explicit UniquePtr(T *ptr = 0, Deleter deleter = Deleter()) throw() : Base(ptr, deleter) {}
		~UniquePtr() {}

		T	&operator[](std::size_t i) const;

	private:
		UniquePtr(const UniquePtr &rhs); 
		UniquePtr &operator=(const UniquePtr &rhs);
};

/**
 * @brief Constructs a UniquePtrBase with a pointer and deleter.
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param ptr The pointer to manage.
 * @param deleter The deleter to use.
 */
template<typename T, typename Deleter>
UniquePtrBase<T, Deleter>::UniquePtrBase(T *ptr, Deleter deleter) throw() : _ptr(ptr), _deleter(deleter) {}

/**
 * @brief Destructor. Deletes the managed pointer using the deleter.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 */
template<typename T, typename Deleter>
UniquePtrBase<T, Deleter>::~UniquePtrBase()
{
	if (this->_ptr)
		this->_deleter(this->_ptr);
}

/**
 * @brief Releases ownership of the managed pointer.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return The managed pointer. After the call, the internal pointer is set to nullptr.
 */
template<typename T, typename Deleter>
T	*UniquePtrBase<T, Deleter>::release() throw()
{
	T *tmp = this->_ptr;
	this->_ptr = 0;
	return (tmp);
}

/**
 * @brief Replaces the managed pointer with a new one, deleting the old one if necessary.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param ptr The new pointer to manage.
 */
template<typename T, typename Deleter>
void	UniquePtrBase<T, Deleter>::reset(T *ptr) throw()
{
	if (this->_ptr != ptr)
	{
		if(this->_ptr)
			this->_deleter(this->_ptr);
		this->_ptr = ptr;
	}
}

/**
 * @brief Swaps the managed pointer and deleter with another UniquePtrBase.
 * 
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @param other The other UniquePtrBase to swap with.
 */
template<typename T, typename Deleter>
void	UniquePtrBase<T, Deleter>::swap(UniquePtrBase &other) throw()
{
	utils::swap(this->_ptr, other._ptr);
	utils::swap(this->_deleter, other._deleter);
}

/**
 * @brief Gets the managed pointer.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return The managed pointer.
 */
template<typename T, typename Deleter>
T	*UniquePtrBase<T, Deleter>::get() const throw()
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
Deleter	&UniquePtrBase<T, Deleter>::getDeleter() throw()
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
const Deleter	&UniquePtrBase<T, Deleter>::getDeleter() const throw()
{
	return (this->_deleter);
}

/**
 * @brief Member access operator for UniquePtr.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Pointer to the managed object.
 */
template<typename T, typename Deleter>
T	*UniquePtr<T, Deleter>::operator->() const throw()
{
	return (this->_ptr);
}

/**
 * @brief Dereference operator for UniquePtr.
 *
 * @tparam T Type of the managed object.
 * @tparam Deleter Type of the deleter functor.
 * @return Reference to the managed object.
 */
template<typename T, typename Deleter>
T	&UniquePtr<T, Deleter>::operator*() const throw()
{
	return (*this->_ptr);
}

/**
 * @brief Array subscript operator for UniquePtr<T[], Deleter>.
 *
 * @tparam T Type of the managed array elements.
 * @tparam Deleter Type of the deleter functor.
 * @param i Index of the element.
 * @return Reference to the element at index i.
 */
template<typename T, typename Deleter>
T	&UniquePtr<T[], Deleter>::operator[](std::size_t i) const
{
	return (this->_ptr[i]);
}

/**
 * @brief Equality comparison operator for UniquePtr.
 *
 * Compares the managed pointers of two UniquePtr instances for equality.
 *
 * @tparam T Type of the first UniquePtr.
 * @tparam TDeleter Deleter type of the first UniquePtr.
 * @tparam U Type of the second UniquePtr.
 * @tparam UDeleter Deleter type of the second UniquePtr.
 * @param lhs Left-hand side UniquePtr.
 * @param rhs Right-hand side UniquePtr.
 * @return true if both UniquePtr instances manage the same pointer, false otherwise.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter> 
bool	operator==(const UniquePtr<T, TDeleter> &lhs, const UniquePtr<U, UDeleter> &rhs)
{
	return (lhs.get() == rhs.get());
}

/**
 * @brief Inequality comparison operator for UniquePtr.
 *
 * Compares the managed pointers of two UniquePtr instances for inequality.
 *
 * @tparam T Type of the first UniquePtr.
 * @tparam TDeleter Deleter type of the first UniquePtr.
 * @tparam U Type of the second UniquePtr.
 * @tparam UDeleter Deleter type of the second UniquePtr.
 * @param lhs Left-hand side UniquePtr.
 * @param rhs Right-hand side UniquePtr.
 * @return true if both UniquePtr instances manage different pointers, false otherwise.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator!=(const UniquePtr<T, TDeleter> &lhs, const UniquePtr<U, UDeleter> &rhs)
{
	return (!(lhs == rhs));
}

/**
 * @brief Less-than comparison operator for UniquePtr.
 *
 * Compares the managed pointers of two UniquePtr instances for ordering.
 *
 * @tparam T Type of the first UniquePtr.
 * @tparam TDeleter Deleter type of the first UniquePtr.
 * @tparam U Type of the second UniquePtr.
 * @tparam UDeleter Deleter type of the second UniquePtr.
 * @param lhs Left-hand side UniquePtr.
 * @param rhs Right-hand side UniquePtr.
 * @return true if lhs's managed pointer is less than rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator<(const UniquePtr<T, TDeleter> &lhs, const UniquePtr<U, UDeleter> &rhs)
{
	return (lhs.get() < rhs.get());
}

/**
 * @brief Greater-than comparison operator for UniquePtr.
 *
 * Compares the managed pointers of two UniquePtr instances for ordering.
 *
 * @tparam T Type of the first UniquePtr.
 * @tparam TDeleter Deleter type of the first UniquePtr.
 * @tparam U Type of the second UniquePtr.
 * @tparam UDeleter Deleter type of the second UniquePtr.
 * @param lhs Left-hand side UniquePtr.
 * @param rhs Right-hand side UniquePtr.
 * @return true if lhs's managed pointer is greater than rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator>(const UniquePtr<T, TDeleter> &lhs, const UniquePtr<U, UDeleter> &rhs)
{
	return (rhs < lhs);
}

/**
 * @brief Less-than-or-equal comparison operator for UniquePtr.
 *
 * Compares the managed pointers of two UniquePtr instances for ordering.
 *
 * @tparam T Type of the first UniquePtr.
 * @tparam TDeleter Deleter type of the first UniquePtr.
 * @tparam U Type of the second UniquePtr.
 * @tparam UDeleter Deleter type of the second UniquePtr.
 * @param lhs Left-hand side UniquePtr.
 * @param rhs Right-hand side UniquePtr.
 * @return true if lhs's managed pointer is less than or equal to rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator<=(const UniquePtr<T, TDeleter> &lhs, const UniquePtr<U, UDeleter> &rhs)
{
	return (!(lhs > rhs));
}

/**
 * @brief Greater-than-or-equal comparison operator for UniquePtr.
 *
 * Compares the managed pointers of two UniquePtr instances for ordering.
 *
 * @tparam T Type of the first UniquePtr.
 * @tparam TDeleter Deleter type of the first UniquePtr.
 * @tparam U Type of the second UniquePtr.
 * @tparam UDeleter Deleter type of the second UniquePtr.
 * @param lhs Left-hand side UniquePtr.
 * @param rhs Right-hand side UniquePtr.
 * @return true if lhs's managed pointer is greater than or equal to rhs's managed pointer.
 */
template<typename T, typename TDeleter, typename U, typename UDeleter>
bool	operator>=(const UniquePtr<T, TDeleter> &lhs, const UniquePtr<U, UDeleter> &rhs)
{
	return (!(lhs < rhs));
}

/**
 * @brief Equality comparison operator for UniquePtr and raw pointer.
 *
 * Compares the managed pointer of a UniquePtr with a raw pointer for equality.
 *
 * @tparam T Type of the UniquePtr.
 * @tparam Deleter Deleter type of the UniquePtr.
 * @param lhs UniquePtr instance.
 * @param null_ptr Raw pointer to compare.
 * @return true if the UniquePtr manages the same pointer as null_ptr.
 */
template<typename T, typename Deleter>
bool operator==(const UniquePtr<T, Deleter> &lhs, const T *null_ptr) throw()
{
	return (lhs.get() == null_ptr);
}

/**
 * @brief Equality comparison operator for raw pointer and UniquePtr.
 *
 * Compares the managed pointer of a UniquePtr with a raw pointer for equality.
 *
 * @tparam T Type of the UniquePtr.
 * @tparam Deleter Deleter type of the UniquePtr.
 * @param null_ptr Raw pointer to compare.
 * @param rhs UniquePtr instance.
 * @return true if the UniquePtr manages the same pointer as null_ptr.
 */
template<typename T, typename Deleter>
bool operator==(const T *null_ptr, const UniquePtr<T, Deleter> &rhs) throw()
{
	return (rhs.get() == null_ptr);
}

/**
 * @brief Inequality comparison operator for UniquePtr and raw pointer.
 *
 * Compares the managed pointer of a UniquePtr with a raw pointer for inequality.
 *
 * @tparam T Type of the UniquePtr.
 * @tparam Deleter Deleter type of the UniquePtr.
 * @param lhs UniquePtr instance.
 * @param null_ptr Raw pointer to compare.
 * @return true if the UniquePtr manages a different pointer than null_ptr.
 */
template<typename T, typename Deleter>
bool operator!=(const UniquePtr<T, Deleter> &lhs, const T *null_ptr) throw()
{
	return (!(lhs == null_ptr));
}

/**
 * @brief Inequality comparison operator for raw pointer and UniquePtr.
 *
 * Compares the managed pointer of a UniquePtr with a raw pointer for inequality.
 *
 * @tparam T Type of the UniquePtr.
 * @tparam Deleter Deleter type of the UniquePtr.
 * @param null_ptr Raw pointer to compare.
 * @param rhs UniquePtr instance.
 * @return true if the UniquePtr manages a different pointer than null_ptr.
 */
template<typename T, typename Deleter>
bool operator!=(const T *null_ptr, const UniquePtr<T, Deleter> &rhs) throw()
{
	return (!(rhs.get() == null_ptr));
}

} // !raii
} // !core
} // !common

#endif // !UNIQUEPTR_HPP
