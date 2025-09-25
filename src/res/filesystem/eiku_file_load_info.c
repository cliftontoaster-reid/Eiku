/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_file_load_info.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:42:07 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:40 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_file_load_info(t_file *file) {
  if (!file || !file->path) {
    errno = EINVAL;
    return false;
  }
  struct stat st;
  if (stat(file->path, &st) != 0) return false;
  file->info = st;
  file->size = st.st_size;
  return true;
}

#endif
