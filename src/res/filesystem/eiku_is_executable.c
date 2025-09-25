/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_is_executable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:42:03 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:42 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_is_executable(const char *path) {
  if (!path) {
    errno = EINVAL;
    return false;
  }
  return access(path, X_OK) == 0;
}

#endif
