// TODO: Don't forget to add 42 header !

/**
 * @file Directory.hpp
 * @brief
 */

#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <dirent.h>
#include <string>

namespace common
{
namespace core
{
namespace utils
{

class DirectoryIterator;

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

#endif // !DIRECTORY_HPP
