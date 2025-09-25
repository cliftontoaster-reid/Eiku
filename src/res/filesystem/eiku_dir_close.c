/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_dir_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:42:59 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:38 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

void eiku_dir_close(t_dir *dir) {
  if (!dir) return;
  if (dir->path) free(dir->path);
  if (dir->entries) {
    for (size_t i = 0; i < dir->count; ++i) free(dir->entries[i]);
    free(dir->entries);
  }
  free(dir);
}

#endif
