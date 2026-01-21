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

#ifndef COMMON_COMMON_HPP
#define COMMON_COMMON_HPP

/**
 * @file common.hpp
 * @brief Common header aggregating core utilities and loader functionality.
 *
 * This header includes various utility and RAII classes, as well as the loader interface,
 * to provide convenient access to commonly used components throughout the project.
 */

#include <common/core/net/sockets/Addrinfo.hpp>
#include <common/core/net/sockets/TcpClient.hpp>
#include <common/core/net/sockets/TcpServer.hpp>

#include <common/core/raii/Deleters.hpp>
#include <common/core/raii/SharedPtr.hpp>
#include <common/core/raii/WeakPtr.hpp>
#include <common/core/raii/UniquePtr.hpp>

#include <common/core/utils/algoUtils.hpp>
#include <common/core/utils/Directory.hpp>
#include <common/core/utils/fileUtils.hpp>
#include <common/core/utils/stringUtils.hpp>
#include <common/core/utils/timeUtils.hpp>

#include <common/loader/Loader.hpp>

#endif // !COMMON_COMMON_HPP

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
