/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_is_symlink.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:40:58 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:43 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_is_symlink(const char *path) {
  if (!path) {
    errno = EINVAL;
    return false;
  }
  struct stat st;
  if (lstat(path, &st) != 0) return false;
  return S_ISLNK(st.st_mode);
}

#endif
