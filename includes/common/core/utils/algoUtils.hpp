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

#ifndef COMMON_ALGOUTILS_HPP
#define COMMON_ALGOUTILS_HPP

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
 * @brief Swaps the values of two objects (or pointers) of type T by reference.
 *
 * This function takes its arguments by reference and swaps their values. If T
 * is a pointer type (for example T = Foo*), the function will swap the
 * pointers themselves (references to pointers).
 *
 * @tparam T Type of the values to swap (may be a pointer type).
 * @param a Reference to the first value.
 * @param b Reference to the second value.
 */
template<typename T>
void	swap(T &a, T &b)
{
	T tmp = a;
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

#endif // !COMMON_ALGOUTILS_HPP

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
