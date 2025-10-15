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

#ifndef DELETERS_HPP
#define DELETERS_HPP

/**
 * @file Deleters.hpp
 * @brief Default deleter functors for use with smart pointers.
 */

/**
* @struct DefaultDelete
* @brief Default deleter for single objects.
*
* Provides a function call operator to delete a pointer to a single object.
*
* @tparam T Type of the object to delete.
*/
template<typename T>
struct DefaultDelete
{
	void	operator()(T *ptr) const throw() { delete ptr; }
};

/**
 * @struct DefaultDelete<T[]>
 * @brief Default deleter specialization for arrays.
 *
 * Provides a function call operator to delete a pointer to an array.
 *
 * @tparam T Type of the array elements to delete.
 */
template<typename T>
struct DefaultDelete<T[]>
{
	void	operator()(T *ptr) const throw() { delete[] ptr; }
};

#endif // !DELETERS_HPP
