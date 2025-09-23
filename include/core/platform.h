/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   platform.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:23:11 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 16:38:22 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// detect OS
#if defined(_WIN32) || defined(_WIN64)
#define EIKU_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#error "Support for iOS/tvOS is not planned!"
#else
#define EIKU_PLATFORM_MACOS
#endif
#elif defined(__linux__)
#define EIKU_PLATFORM_LINUX
#elif defined(__unix__)
#define EIKU_PLATFORM_UNIX
#else
#error "Unsupported platform!"
#endif

// detect architecture
#if defined(_M_X64) || defined(__x86_64__)
#define EIKU_ARCH_X86_64
#elif defined(_M_IX86) || defined(__i386__)
#define EIKU_ARCH_X86
#elif defined(__aarch64__)
#define EIKU_ARCH_ARM64
#elif defined(__arm__)
#define EIKU_ARCH_ARM
#else
#warning "Unknown architecture"
#endif

// helpers for export/import
#if defined(EIKU_PLATFORM_WINDOWS)
#if defined(EIKU_BUILD_DLL)
#define EIKU_API __declspec(dllexport)
#else
#define EIKU_API __declspec(dllimport)
#endif
#else
#define EIKU_API __attribute__((visibility("default")))
#endif

// helpers for internal functions
#if defined(EIKU_PLATFORM_WINDOWS)
#define EIKU_INT
#else
#define EIKU_INT __attribute__((visibility("hidden")))
#endif
