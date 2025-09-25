/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:39:59 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:40 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

t_file *eiku_file_open(const char *path, t_file_mode mode) {
  if (!path) {
    errno = EINVAL;
    return NULL;
  }

  int flags = 0;
  switch (mode) {
    case EIKU_FS_READ:
      flags = O_RDONLY;
      break;
    case EIKU_FS_WRITE:
      /* default to create/truncate for write mode */
      flags = O_WRONLY | O_CREAT | O_TRUNC;
      break;
    default:
      errno = EINVAL;
      return NULL;
  }

  int fd = open(path, flags, 0644);
  if (fd < 0) return NULL;

  t_file *file = malloc(sizeof(*file));
  if (!file) {
    close(fd);
    errno = ENOMEM;
    return NULL;
  }
  memset(file, 0, sizeof(*file));
  file->fd = fd;
  file->path = strdup(path);
  file->size = 0;
  file->content = NULL;
  /* info left zeroed until eiku_file_load_info is called */
  return file;
}

#endif
