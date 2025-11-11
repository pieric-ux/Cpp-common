/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdemont <pdemont@student.42lausanne.ch>    +#+  +:+       +#+        */
/*   By: blucken <blucken@student.42lausanne.ch>  +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*   Created: 2025/10/16                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
#define COMMON_HPP

/**
 * @file common.hpp
 * @brief Common header aggregating core utilities and loader functionality.
 *
 * This header includes various utility and RAII classes, as well as the loader interface,
 * to provide convenient access to commonly used components throughout the project.
 */

#include "core/raii/Deleters.hpp"
#include "core/raii/SharedPtr.hpp"
#include "core/raii/WeakPtr.hpp"
#include "core/raii/UniquePtr.hpp"

#include "core/utils/algoUtils.hpp"
#include "core/utils/Directory.hpp"
#include "core/utils/fileUtils.hpp"
#include "core/utils/stringUtils.hpp"
#include "core/utils/timeUtils.hpp"

#include "loader/Loader.hpp"

using namespace common::core;

#endif // !COMMON_HPP
