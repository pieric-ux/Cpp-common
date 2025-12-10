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

} // !raii
} // !core
} // !common
#endif // !COMMON_DELETERS_HPP
