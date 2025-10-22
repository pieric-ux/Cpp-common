/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file Loader.cpp
 * @brief Implementation of the Loader class for loading files from directories.
 */

#include "core/utils/Directory.hpp"
#include "core/utils/fileUtils.hpp"
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
 * @brief Default constructor for Loader.
 */
Loader::Loader() {}

/**
 * @brief Destructor for Loader.
 */
Loader::~Loader() {}

/**
 * @brief Loads all files with a specific extension from a directory and parses them.
 *
 * Iterates through the specified directory, and for each regular file with the given extension,
 * calls the provided parser function on the file.
 *
 * @param dirname The path to the directory to load files from.
 * @param extension The file extension to filter files.
 * @param parser The function to parse each file, taking an input stream and filename.
 * @throws std::runtime_error If a file cannot be opened.
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
 * @brief Loads and parses a single file.
 *
 * Opens the specified file and passes its input stream to the parser function.
 *
 * @param filename The path to the file to load.
 * @param parser The function to parse the file, taking an input stream and filename.
 * @throws std::ios_base::failure If the file cannot be opened.
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
