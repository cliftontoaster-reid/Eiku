/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:22:50 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 23:54:36 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// bump these per release
#define EIKU_VERSION_MAJOR 0
#define EIKU_VERSION_MINOR 1
#define EIKU_VERSION_PATCH 0
#define EIKU_VERSION_SUFFIX "exp"
#define EIKU_VERSION_BUILD 1

// helper macros
#define _EIKU_STR(x) #x
#define EIKU_STR(x) _EIKU_STR(x)

// full-string: "0.1.0-exp.1"
#define EIKU_VERSION_BASE      \
  EIKU_STR(EIKU_VERSION_MAJOR) \
  "." EIKU_STR(EIKU_VERSION_MINOR) "." EIKU_STR(EIKU_VERSION_PATCH)

/*
  If EIKU_VERSION_SUFFIX is not an empty string literal (i.e. sizeof("")==1),
  append -SUFFIX.BUILD to the base version. We use sizeof(EIKU_VERSION_SUFFIX)
  in a ternary expression which is evaluated at compile time. This keeps
  `EIKU_VERSION_STRING` usable as a single C string expression.
*/
#define EIKU_VERSION_STRING                                     \
  (sizeof(EIKU_VERSION_SUFFIX) > 1 ? EIKU_VERSION_BASE          \
       "-" EIKU_VERSION_SUFFIX "." EIKU_STR(EIKU_VERSION_BUILD) \
                                   : EIKU_VERSION_BASE)

// optional: encode to single integer for comparisons:
// e.g. (major<<16)|(minor<<8)|patch
#define EIKU_VERSION_CODE                                   \
  ((EIKU_VERSION_MAJOR << 16) | (EIKU_VERSION_MINOR << 8) | \
   (EIKU_VERSION_PATCH))