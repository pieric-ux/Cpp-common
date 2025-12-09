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

#include "common/core/raii/Deleters.hpp"
#include "common/core/raii/SharedPtr.hpp"
#include "common/core/raii/WeakPtr.hpp"
#include "common/core/raii/UniquePtr.hpp"

#include "common/core/utils/algoUtils.hpp"
#include "common/core/utils/Directory.hpp"
#include "common/core/utils/fileUtils.hpp"
#include "common/core/utils/stringUtils.hpp"
#include "common/core/utils/timeUtils.hpp"

#include "common/loader/Loader.hpp"

#endif // !COMMON_HPP
