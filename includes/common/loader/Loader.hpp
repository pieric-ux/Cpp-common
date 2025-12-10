/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file Loader.hpp
 * @brief Declaration of the Loader class for loading and parsing files from directories.
 */

#ifndef COMMON_LOADER_HPP
#define COMMON_LOADER_HPP

#include <fstream>
#include <string>

namespace common
{
namespace loader
{

/**
 * @class Loader
 * @brief Utility class for loading files from directories and parsing them.
 *
 * The Loader class provides methods to iterate over files in a directory with a specific extension,
 * and to apply a user-provided parser function to each file.
 */
class Loader
{
	public:
		Loader();
		~Loader();

		typedef void (*fileParser)(std::ifstream &ifs, const std::string &filename);

		void	loadDirectory(const std::string &dirname, const std::string &extension, fileParser parser);

	private:
		Loader(const Loader &rhs);
		Loader &operator=(const Loader &rhs);

		void	loadFile(const std::string &filename, fileParser parser);
};

} // !loader
} // !common

#endif // !COMMON_LOADER_HPP
