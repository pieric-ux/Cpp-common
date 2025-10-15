// TODO: Don't forget to add 42 header !

#ifndef DELETERS_HPP
#define DELETERS_HPP

/**
 * @file UniquePtr.hpp
 * @brief
 */

/**
 * @brief 
 *
 * @tparam T 
 * @param ptr 
 */
template<typename T>
struct DefaultDelete
{
	void	operator()(T *ptr) const throw() { delete ptr; }
};

/**
 * @brief 
 *
 * @tparam T 
 * @param ptr 
 */
template<typename T>
struct DefaultDelete<T[]>
{
	void	operator()(T *ptr) const throw() { delete[] ptr; }
};

#endif // !DELETERS_HPP
