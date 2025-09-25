/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_get_file_extension.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:43:20 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:42 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

char *eiku_get_file_extension(const char *filename) {
  if (!filename) return NULL;
  const char *p = strrchr(filename, '.');
  if (!p || p == filename) return NULL; /* dotfile or no extension */
  return strdup(p + 1);
}

#endif
