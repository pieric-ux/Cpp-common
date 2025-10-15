// TODO: Don't forget to add 42 header !

#ifndef ALGOUTILS_HPP
#define ALGOUTILS_HPP

/**
 * @file UniquePtr.hpp
 * @brief
 */

#include <cstddef>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief 
 *
 * @tparam T 
 * @param a 
 * @param b 
 */
template<typename T>
void	swap(T *a, T *b)
{
	T *tmp = a;
	a = b;
	b = tmp;
}

/**
 * @brief 
 *
 * @tparam T 
 * @param a 
 * @param N 
 * @param b 
 * @param N 
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
