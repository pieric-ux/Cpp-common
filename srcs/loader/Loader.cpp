// TODO: Don't forget to add 42 header !

/**
 * @file Loader.cpp
 * @brief
 */

#include "common/core/utils/Directory.hpp"
#include "common/core/utils/fileUtils.hpp"
#include "loader/Loader.hpp"
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>

namespace common
{
namespace loader 
{

/**
 * @brief 
 */
Loader::Loader() {}

/**
 * @brief 
 */
Loader::~Loader() {}

/**
 * @brief 
 *
 * @param dirname 
 */
void	Loader::loadDirectory(const std::string &dirname, const std::string &extension, fileParser parser)
{
	core::utils::Directory dir(dirname);

	for (core::utils::DirectoryIterator it = dir.begin(); it != dir.end(); ++it)
	{
		struct dirent *entry = *it;
		if (entry->d_type == DT_REG && core::utils::hasExtension(entry->d_name, extension))
		{
			std::string filename = dirname + "/" + entry->d_name;
			try
			{
				this->loadFile(filename, parser);
			}
			catch (std::ios_base::failure &e)
			{
				throw std::runtime_error("Unable to open file " + filename + ": " + strerror(errno));
			}
		}
	}
}

/**
 * @brief 
 *
 * @param string 
 */
void	Loader::loadFile(const std::string &filename, fileParser parser)
{
	std::ifstream	ifs;

	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	ifs.open(filename.c_str());

	parser(ifs, filename);
}

} // !loader
} // !common
