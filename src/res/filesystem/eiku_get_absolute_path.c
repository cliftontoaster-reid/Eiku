/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_get_absolute_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:43:52 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:41 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

/* Returns a lexical absolute path: if `path` is relative, join with cwd and
   normalize. Does not resolve symlinks and works for non-existent paths. */
char *eiku_get_absolute_path(const char *path) {
  if (!path) return NULL;
  if (path[0] == '/') return strdup(path);
  char cwd[PATH_MAX];
  if (!getcwd(cwd, sizeof(cwd))) return NULL;
  size_t need = strlen(cwd) + 1 + strlen(path) + 1;
  char *buf = malloc(need);
  if (!buf) return NULL;
  strcpy(buf, cwd);
  strcat(buf, "/");
  strcat(buf, path);
  char *norm = eiku_normalize_path(buf);
  free(buf);
  return norm;
}

#endif
