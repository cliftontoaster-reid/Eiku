/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_get_dirname.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:43:12 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:42 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

char *eiku_get_dirname(const char *path) {
  if (!path) return NULL;
  const char *p = strrchr(path, '/');
  if (!p) return strdup(".");
  if (p == path) return strdup("/");
  size_t len = p - path;
  char *out = malloc(len + 1);
  if (!out) return NULL;
  memcpy(out, path, len);
  out[len] = '\0';
  return out;
}

#endif
