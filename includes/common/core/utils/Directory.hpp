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
