/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:44:24 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:41 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

ssize_t eiku_file_read(t_file *file, uint8_t *buffer, size_t n) {
  if (!file || file->fd < 0 || !buffer) {
    errno = EINVAL;
    return -1;
  }
  ssize_t total = 0;
  while (total < (ssize_t)n) {
    ssize_t r = read(file->fd, buffer + total, n - total);
    if (r < 0) {
      if (errno == EINTR) continue;
      return -1;
    }
    if (r == 0) break;
    total += r;
  }
  return total;
}

#endif
