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
 *
 * @startuml
 * class "Loader" as Loader {
		+ <<typedef>> fileParser
		--
		+ Loader()
		+ loadDirectory(dirname : string, extension : string, parser : fileParser) : void
		- loadFile(filename : string, parser : fileParser) : void
	}
 * @enduml
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
