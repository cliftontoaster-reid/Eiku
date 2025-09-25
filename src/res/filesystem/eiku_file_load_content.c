/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_load_content.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:42:11 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:39 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_file_load_content(t_file *file) {
  if (!file || !file->path || file->fd < 0) {
    errno = EINVAL;
    return false;
  }

  /* Ensure we have size info */
  if (file->size == 0) {
    if (!eiku_file_load_info(file)) return false;
  }

  if (file->size < 0) {
    errno = EINVAL;
    return false;
  }

  /* allocate buffer (plus one NUL for convenience) */
  uint8_t *buf = malloc((size_t)file->size + 1);
  if (!buf) {
    errno = ENOMEM;
    return false;
  }

  /* read from the beginning */
  if (lseek(file->fd, 0, SEEK_SET) == (off_t)-1) {
    free(buf);
    return false;
  }

  ssize_t toread = (ssize_t)file->size;
  ssize_t off = 0;
  while (off < toread) {
    ssize_t r = read(file->fd, buf + off, (size_t)(toread - off));
    if (r < 0) {
      if (errno == EINTR) continue;
      free(buf);
      return false;
    }
    if (r == 0) break;
    off += r;
  }
  buf[off] = 0;
  /* free any previous content */
  if (file->content) free(file->content);
  file->content = buf;
  return true;
}

#endif
