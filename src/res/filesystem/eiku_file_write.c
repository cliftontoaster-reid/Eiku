/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:44:32 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:41 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

ssize_t eiku_file_write(t_file *file, const uint8_t *buffer, size_t n) {
  if (!file || file->fd < 0 || !buffer) {
    errno = EINVAL;
    return -1;
  }
  ssize_t total = 0;
  while (total < (ssize_t)n) {
    ssize_t w = write(file->fd, buffer + total, n - total);
    if (w < 0) {
      if (errno == EINTR) continue;
      return -1;
    }
    total += w;
  }
  return total;
}

#endif
