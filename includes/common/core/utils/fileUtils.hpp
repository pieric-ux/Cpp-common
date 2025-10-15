/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file fileUtils.hpp
 * @brief Utility functions for file name and path manipulations.
 */

#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

namespace common
{
namespace core
{
namespace utils 
{

bool		hasExtension(const std::string &filename, const std::string &ext);
std::string	filenameNoExt(const std::string &filename, const std::string &ext);
std::string	filenameNoPath(const std::string &path);

} // !utils
} // !core
} // !common

#endif // !FILEUTILS_HPP
