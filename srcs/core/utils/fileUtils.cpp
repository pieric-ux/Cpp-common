/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file fileUtils.cpp
 * @brief Utility functions for file name and path manipulations.
 */

#include <string>

namespace common
{
namespace core
{
namespace utils
{

/**
 * @brief Checks if a filename ends with the specified extension.
 *
 * @param filename The name of the file to check.
 * @param ext The extension to check for (including the dot, e.g., ".txt").
 * @return true if the filename ends with the given extension, false otherwise.
 */
bool	hasExtension(const std::string &filename, const std::string &ext)
{
	return (
		filename.size() >= ext.size() 
		&& filename.substr(filename.size() - ext.size()) == ext
	);
}

/**
 * @brief Returns the filename without the specified extension if it exists.
 *
 * @param filename The name of the file.
 * @param ext The extension to remove (including the dot, e.g., ".txt").
 * @return The filename without the extension, or the original filename if the extension is not present.
 */
std::string	filenameNoExt(const std::string &filename, const std::string &ext)
{
	std::size_t pos = filename.rfind(ext); 
	if (pos != std::string::npos && pos == filename.size() - ext.size())
		return (filename.substr(0, pos));
	return (filename);
}

/**
 * @brief Extracts the filename from a full path.
 *
 * @param path The full path to extract the filename from.
 * @return The filename without the preceding path.
 */
std::string	filenameNoPath(const std::string &path)
{
	std::size_t pos = path.rfind('/');
	if (pos == std::string::npos)
		return (path);
	return (path.substr(pos + 1));
}

} // !utils
} // !core
} // !common
