/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_get_data_addr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+
        +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:35:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/22 19:35:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <stdlib.h>

EIKU_API char *eiku_get_data_addr(t_eiku_image *img, int *bits_per_pixel,
                                  int *size_line, int *endian) {
  if (!img) return (NULL);
  if (bits_per_pixel) *bits_per_pixel = img->bpp;
  if (size_line) *size_line = img->size_line;
  if (endian)
    *endian = 0; /* XImage uses native endian; return 0 as placeholder */
  return (img->data);
}

#else
EIKU_API char *eiku_get_data_addr(t_eiku_image *img, int *bits_per_pixel,
                                  int *size_line, int *endian) {
  (void)img;
  (void)bits_per_pixel;
  (void)size_line;
  (void)endian;
  return (NULL);
}
#endif
