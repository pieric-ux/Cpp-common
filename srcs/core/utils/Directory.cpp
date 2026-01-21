/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directory.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file Directory.cpp
 * @brief Implementation of the Directory and DirectoryIterator classes for directory traversal utilities.
 */

#include <common/core/utils/Directory.hpp>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <string>
#include <sys/stat.h>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief Constructs a Directory object and opens the specified directory.
 *
 * @param filename Path to the directory to open.
 * @throws std::runtime_error If the directory cannot be opened.
 */
Directory::Directory(const std::string &filename) : dir(NULL)
{
	this->dir = opendir(filename.c_str());
	if (!dir)
		throw std::runtime_error("Unable to open directory " + filename + ": " + std::strerror(errno));
}

/**
 * @brief Destructor for Directory. Closes the opened directory if necessary.
 */
Directory::~Directory()
{
	if (this->dir)
		closedir(this->dir);
}

/**
 * @brief Returns an iterator to the beginning of the directory.
 *
 * @return DirectoryIterator pointing to the first entry.
 */
DirectoryIterator	Directory::begin()
{
	return (DirectoryIterator(this));
}

/**
 * @brief Returns an iterator to the end of the directory.
 *
 * @return DirectoryIterator representing the end.
 */
DirectoryIterator	Directory::end()
{
	return (DirectoryIterator());
}

/**
 * @brief Gets the underlying DIR pointer.
 *
 * @return Pointer to DIR structure.
 */
DIR *Directory::getDir() const
{
	return (this->dir);
}

/**
 * @brief Creates a directory if it does not exist.
 *
 * @param filename Path to the directory to create.
 * @throws std::runtime_error If the path exists but is not a directory, or if creation fails.
 */
void	Directory::create(const std::string &filename)
{
	struct stat info;
	
	if (stat(filename.c_str(), &info) == 0)
	{
		if (info.st_mode & S_IFDIR)
			return ;
		else
			throw std::runtime_error("`" + filename + "` exists but is not a directory");
	}
		
	if (mkdir(filename.c_str(), 0777) != 0)
		throw std::runtime_error("Unable to create directory `" + filename + "`: " + std::strerror(errno));
}

/**
 * @brief Default constructor for DirectoryIterator. Creates an end iterator.
 */
DirectoryIterator::DirectoryIterator() : dir(NULL), entry(NULL) {}

/**
 * @brief Constructs a DirectoryIterator for the given Directory.
 *
 * @param init_dir Pointer to the Directory to iterate.
 */
DirectoryIterator::DirectoryIterator(Directory *init_dir) : dir(init_dir), entry(NULL)
{
	if (this->dir)
		++(*this);
}

/**
 * @brief Destructor for DirectoryIterator.
 */
DirectoryIterator::~DirectoryIterator() {}

/**
 * @brief Copy constructor for DirectoryIterator.
 *
 * @param rhs DirectoryIterator to copy from.
 */
DirectoryIterator::DirectoryIterator(const DirectoryIterator &rhs) : dir(rhs.dir), entry(rhs.entry) {}

/**
 * @brief Assignment operator for DirectoryIterator.
 *
 * @param rhs DirectoryIterator to assign from.
 * @return Reference to this iterator.
 */
DirectoryIterator	&DirectoryIterator::operator=(const DirectoryIterator &rhs)
{
	if (this != &rhs)
	{
		this->dir = rhs.dir;
		this->entry = rhs.entry;
	}
	return (*this);
}

/**
 * @brief Dereference operator.
 *
 * @return Pointer to the current dirent structure.
 */
struct dirent *DirectoryIterator::operator*() const
{
	return (this->entry);
}

/**
 * @brief Pre-increment operator. Advances to the next directory entry.
 *
 * @return Reference to this iterator.
 * @throws std::runtime_error If reading the directory fails.
 */
DirectoryIterator &DirectoryIterator::operator++()
{
	if(this->dir->getDir())
	{
		errno = 0;
		this->entry = readdir(this->dir->getDir());
		if (!entry && errno)
			throw std::runtime_error("Unable to read directory: " + std::string(std::strerror(errno)));
	}
	return (*this);
}

/**
 * @brief Post-increment operator. Advances to the next directory entry.
 *
 * @return Copy of the iterator before increment.
 */
DirectoryIterator DirectoryIterator::operator++(int)
{
	DirectoryIterator tmp = *this;
	++(*this);
	return (tmp);
}

/**
 * @brief Inequality comparison operator.
 *
 * @param rhs DirectoryIterator to compare with.
 * @return True if iterators are not equal, false otherwise.
 */
bool DirectoryIterator::operator!=(const DirectoryIterator &rhs) const
{
	return (this->entry != rhs.entry);
}

} // !utils
} // !core
} // !common

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
