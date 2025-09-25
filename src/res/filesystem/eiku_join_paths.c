/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_join_paths.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:43:24 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:43 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

char *eiku_join_paths(const char *path1, const char *path2) {
  if (!path1 || !path2) return NULL;
  /* if path2 is absolute, return a copy */
  if (path2[0] == '/') return strdup(path2);
  size_t l1 = strlen(path1);
  bool need_slash = (l1 == 0 || path1[l1 - 1] != '/');
  size_t out_len = l1 + (need_slash ? 1 : 0) + strlen(path2) + 1;
  char *out = malloc(out_len);
  if (!out) return NULL;
  strcpy(out, path1);
  if (need_slash) strcat(out, "/");
  strcat(out, path2);
  return out;
}

#endif
