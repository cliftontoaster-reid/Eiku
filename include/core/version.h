/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:22:50 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/21 19:55:57 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// bump these per release
#define EIKU_VERSION_MAJOR 0
#define EIKU_VERSION_MINOR 1
#define EIKU_VERSION_PATCH 0

// helper macros
#define _EIKU_STR(x) #x
#define EIKU_STR(x) _EIKU_STR(x)

// full-string: "0.1.0"
#define EIKU_VERSION_STRING    \
  EIKU_STR(EIKU_VERSION_MAJOR) \
  "." EIKU_STR(EIKU_VERSION_MINOR) "." EIKU_STR(EIKU_VERSION_PATCH)

// optional: encode to single integer for comparisons:
// e.g. (major<<16)|(minor<<8)|patch
#define EIKU_VERSION_CODE                                   \
  ((EIKU_VERSION_MAJOR << 16) | (EIKU_VERSION_MINOR << 8) | \
   (EIKU_VERSION_PATCH))