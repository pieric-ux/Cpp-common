/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directory.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_DIRECTORY_HPP
#define COMMON_DIRECTORY_HPP

/**
 * @file Directory.hpp
 * @brief Directory traversal utilities for iterating and manipulating directories.
 */

#include <dirent.h>
#include <string>

namespace common
{
namespace core
{
namespace utils
{

class DirectoryIterator;

/**
 * @class Directory
 * @brief RAII wrapper for directory operations.
 *
 * Provides methods to open, iterate, and create directories using POSIX APIs.
 */
class Directory
{
	public:
		explicit Directory(const std::string &filename);
		~Directory();

		DirectoryIterator begin();
		DirectoryIterator end();

		DIR *getDir() const;

		static void create(const std::string &filename);

	private:
		DIR *dir;

		Directory(const Directory &rhs);
		Directory &operator=(const Directory &rhs);
};

/**
 * @class DirectoryIterator
 * @brief Iterator for traversing directory entries.
 *
 * Provides standard iterator operations for iterating over directory entries.
 */
class DirectoryIterator
{
	public:
		DirectoryIterator();
		explicit DirectoryIterator(Directory *dir);
		~DirectoryIterator();

		DirectoryIterator(const DirectoryIterator &rhs);
		DirectoryIterator &operator=(const DirectoryIterator &rhs);

		struct dirent *operator*() const;
		DirectoryIterator &operator++();
		DirectoryIterator operator++(int);
		bool operator!=(const DirectoryIterator &rhs) const;

	private:
		Directory *dir;
		struct dirent *entry;
};

} // !utils
} // !core
} // !common

#endif // !COMMON_DIRECTORY_HPP

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
