/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_is_readable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:41:52 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:43 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

bool eiku_is_readable(const char *path) {
  if (!path) {
    errno = EINVAL;
    return false;
  }
  return access(path, R_OK) == 0;
}

#endif
