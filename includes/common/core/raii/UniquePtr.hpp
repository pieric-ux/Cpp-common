// TODO: Don't forget to add 42 header !

#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP

/**
 * @file UniquePtr.hpp
 * @brief
 */

#include "../utils/algoUtils.hpp"
#include "Deleters.hpp"
#include <cstddef>

namespace common
{
namespace core
{
namespace raii
{

/**
 * @brief 
 *
 * @tparam T 
 * @param ptr 
 * @return 
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
 * @brief 
 *
 * @tparam T 
 * @param ptr 
 * @return 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param ptr 
 * @param deleter 
 * @return 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param ptr 
 * @param deleter 
 * @return 
 */
template<typename T, typename Deleter>
UniquePtrBase<T, Deleter>::UniquePtrBase(T *ptr, Deleter deleter) throw() : _ptr(ptr), _deleter(deleter) {}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
UniquePtrBase<T, Deleter>::~UniquePtrBase()
{
	if (this->_ptr)
		this->_deleter(this->_ptr);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
T	*UniquePtrBase<T, Deleter>::release() throw()
{
	T *tmp = this->_ptr;
	this->_ptr = 0;
	return (tmp);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param ptr 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param other 
 */
template<typename T, typename Deleter>
void	UniquePtrBase<T, Deleter>::swap(UniquePtrBase &other) throw()
{
	utils::swap(this->_ptr, other._ptr);
	utils::swap(this->_deleter, other._deleter);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
T	*UniquePtrBase<T, Deleter>::get() const throw()
{
	return (this->_ptr);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
Deleter	&UniquePtrBase<T, Deleter>::getDeleter() throw()
{
	return (this->_deleter);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
const Deleter	&UniquePtrBase<T, Deleter>::getDeleter() const throw()
{
	return (this->_deleter);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
T	*UniquePtr<T, Deleter>::operator->() const throw()
{
	return (this->_ptr);
}

/**
 * @brief 
 *
 * @tparam T 
 * @return 
 */
template<typename T, typename Deleter>
T	&UniquePtr<T, Deleter>::operator*() const throw()
{
	return (*this->_ptr);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param i 
 * @return 
 */
template<typename T, typename Deleter>
T	&UniquePtr<T[], Deleter>::operator[](std::size_t i) const
{
	return (this->_ptr[i]);
}

} // !raii
} // !core
} // !common

#endif // !UNIQUEPTR_HPP
