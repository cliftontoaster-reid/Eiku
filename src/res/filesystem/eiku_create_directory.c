/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_create_directory.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:44:03 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:37 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/stat.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_create_directory(const char *path) {
  if (!path) {
    errno = EINVAL;
    return false;
  }
  if (mkdir(path, 0755) != 0) return false;
  return true;
}

#endif
