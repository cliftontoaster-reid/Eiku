/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:40:28 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:39 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

void eiku_file_close(t_file *file) {
  if (!file) return;
  if (file->fd >= 0) close(file->fd);
  if (file->path) free(file->path);
  if (file->content) free(file->content);
  /* struct stat info is embedded, no free needed */
  free(file);
}

#endif
