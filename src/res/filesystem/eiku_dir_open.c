/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_dir_open.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:42:37 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:38 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

t_dir *eiku_dir_open(const char *path) {
  if (!path) {
    errno = EINVAL;
    return NULL;
  }
  DIR *d = opendir(path);
  if (!d) return NULL;

  t_dir *dir = malloc(sizeof(*dir));
  if (!dir) {
    closedir(d);
    errno = ENOMEM;
    return NULL;
  }
  dir->path = strdup(path);
  dir->entries = NULL;
  dir->count = 0;

  struct dirent *ent;
  while ((ent = readdir(d)) != NULL) {
    /* skip . and .. */
    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
      continue;
    char **new_entries =
        realloc(dir->entries, (dir->count + 1) * sizeof(*dir->entries));
    if (!new_entries) {
      /* cleanup */
      for (size_t i = 0; i < dir->count; ++i) free(dir->entries[i]);
      free(dir->entries);
      free(dir->path);
      free(dir);
      closedir(d);
      errno = ENOMEM;
      return NULL;
    }
    dir->entries = new_entries;
    dir->entries[dir->count] = strdup(ent->d_name);
    if (!dir->entries[dir->count]) {
      /* cleanup */
      for (size_t i = 0; i < dir->count; ++i) free(dir->entries[i]);
      free(dir->entries);
      free(dir->path);
      free(dir);
      closedir(d);
      errno = ENOMEM;
      return NULL;
    }
    dir->count++;
  }

  closedir(d);
  return dir;
}

#endif
