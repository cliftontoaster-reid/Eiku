/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_get_filename.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:43:08 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:42 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

char *eiku_get_filename(const char *path) {
  if (!path) return NULL;
  const char *p = strrchr(path, '/');
  if (!p) return strdup(path);
  /* skip trailing slash case "/foo/" -> filename "" -> return NULL */
  if (*(p + 1) == '\0') return NULL;
  return strdup(p + 1);
}

#endif
