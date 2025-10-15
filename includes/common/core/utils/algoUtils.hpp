/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algoUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALGOUTILS_HPP
#define ALGOUTILS_HPP

/**
 * @file UniquePtr.hpp
 * @brief Utility algorithms for generic operations such as swapping values and arrays.
 */

#include <cstddef>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief Swaps the values of two pointers of type T.
 *
 * @tparam T Type of the pointers to swap.
 * @param a Pointer to the first value.
 * @param b Pointer to the second value.
 */
template<typename T>
void	swap(T *a, T *b)
{
	T *tmp = a;
	a = b;
	b = tmp;
}

/**
 * @brief Swaps the contents of two arrays of type T and size N.
 *
 * @tparam T Type of the array elements.
 * @tparam N Size of the arrays.
 * @param a Reference to the first array.
 * @param b Reference to the second array.
 */
template<typename T, std::size_t N>
void	swap(T (&a)[N], T (&b)[N])
{
	for (std::size_t i = 0; i < N; ++i)
	{
		swap(a[i], b[i]);
	}
}

} // !utils
} // !core
} // !common

#endif // !ALGOUTILS_HPP
