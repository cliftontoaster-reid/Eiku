/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_remove_directory.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:44:14 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:44 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_remove_directory(const char *path) {
  if (!path) {
    errno = EINVAL;
    return false;
  }
  if (rmdir(path) != 0) return false;
  return true;
}

#endif
