/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_seek.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:44:39 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:41 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

off_t eiku_file_seek(t_file *file, off_t offset, int whence) {
  if (!file || file->fd < 0) {
    errno = EINVAL;
    return (off_t)-1;
  }
  off_t res = lseek(file->fd, offset, whence);
  return res;
}

#endif
