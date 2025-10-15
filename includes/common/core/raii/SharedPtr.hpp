// TODO: Don't forget to add 42 header !

#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

/**
 * @file SharedPtr.hpp
 * @brief
 */

#include "../utils/algoUtils.hpp"
#include "../raii/Deleters.hpp"
#include <cstddef>

namespace common
{
namespace core
{
namespace raii
{

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
 * @brief 
 *
 * @tparam T 
 * @param ptr 
 * @param deleters 
 * @return 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param ptr 
 * @param deleters 
 * @return 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param ptr 
 * @param deleter 
 * @return 
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::SharedPtrBase(T *ptr, Deleter deleter) throw() : _ptr(ptr), _deleter(deleter), _count(0)
{
	if (this->_ptr)
		this->_count = new std::size_t(1);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::~SharedPtrBase()
{
	reset();
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param rhs 
 * @return 
 */
template<typename T, typename Deleter>
SharedPtrBase<T, Deleter>::SharedPtrBase(const SharedPtrBase &rhs) throw() : _ptr(rhs._ptr), _deleter(rhs._deleter), _count(rhs._count)
{
	if (this->_count)
		++(this->_count);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param rhs 
 * @return 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param ptr 
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
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @param other 
 */
template<typename T, typename Deleter>
void	SharedPtrBase<T, Deleter>::swap(SharedPtrBase &other) throw()
{
	utils::swap(this->_ptr, other._ptr);
	utils::swap(this->_deleter, other._deleter);
	utils::swap(this->_count, other._count);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
T	*SharedPtrBase<T, Deleter>::get() const throw()
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
Deleter	&SharedPtrBase<T, Deleter>::getDeleter() throw()
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
const Deleter	&SharedPtrBase<T, Deleter>::getDeleter() const throw()
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
std::size_t		SharedPtrBase<T, Deleter>::useCount() const throw()
{
	return (this->_count ? *this->_count : 0);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
bool	SharedPtrBase<T, Deleter>::unique() const throw()
{
	return (this->useCount() == 1);
}

/**
 * @brief 
 *
 * @tparam T 
 * @tparam Deleter 
 * @return 
 */
template<typename T, typename Deleter>
T	*SharedPtr<T, Deleter>::operator->() const throw()
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
T	&SharedPtr<T, Deleter>::operator*() const throw()
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
T	&SharedPtr<T[], Deleter>::operator[](std::size_t i) const
{
	return (this->_ptr[i]);
}

} // !raii
} // !core
} // !common

#endif // !SHAREDPTR_HPP
