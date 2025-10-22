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
 * @struct IDeleter
 * @brief 
 *
 */
struct IDeleter
{
	virtual ~IDeleter() {}

	virtual void destroy(void *ptr) = 0;
};

/**
* @struct DefaultDelete
* @brief Default deleter for single objects.
*
* Provides a function call operator to delete a pointer to a single object.
*
* @tparam T Type of the object to delete.
*/
template<typename T>
struct DefaultDelete : public IDeleter
{
	void	destroy(void *ptr) const throw() { delete static_cast<T *>(ptr); }
};

#endif // !DELETERS_HPP
