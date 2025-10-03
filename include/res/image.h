/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.h                                             :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:00:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/22 18:00:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** EIKU Image Module
**
** This module provides image creation, manipulation, and rendering functions
** for the Eiku game engine. It handles X11 image buffers, pixel operations,
** and image display functionality.
**
** The implementation is inspired by MiniLibX but written from scratch
** for the Eiku engine.
*/

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#endif

// Forward declarations for types defined in core.h
typedef struct s_eiku_context t_eiku_context;
typedef struct s_eiku_window t_eiku_window;

// Image structure for X11 images
typedef struct s_eiku_image {
  XImage *image;
  Pixmap pix;
  GC gc;
  int size_line;
  int bpp;
  int width;
  int height;
  int type;
  int format;
  char *data;
  XShmSegmentInfo shm;
} t_eiku_image;

// Image creation and destruction functions
EIKU_API t_eiku_image *eiku_new_image(t_eiku_context *ctx, int width,
                                      int height);
EIKU_API int eiku_destroy_image(t_eiku_context *ctx, t_eiku_image *img);
EIKU_API char *eiku_get_data_addr(t_eiku_image *img, int *bits_per_pixel,
                                  int *size_line, int *endian);

// Pixel manipulation functions
EIKU_API int eiku_pixel_put(t_eiku_context *ctx, t_eiku_window *win, int x,
                            int y, int color);
EIKU_API int eiku_put_image_to_window(t_eiku_context *ctx, t_eiku_window *win,
                                      t_eiku_image *img, int x, int y);
// Image-only pixel write: put pixel directly into image buffer
EIKU_API int eiku_image_pixel_put(t_eiku_image *img, int x, int y, int color);
// Returns the pixel color at (x, y) in the image in the RRGGBB format
EIKU_API int eiku_image_pixel_get(t_eiku_image *img, int x, int y);

// Color functions
EIKU_API int eiku_get_color_value(t_eiku_context *ctx, int color);

// XPM image loading functions (optional - can be implemented later)
EIKU_API t_eiku_image *eiku_xpm_file_to_image(t_eiku_context *ctx,
                                              const char *filename);
EIKU_API t_eiku_image *eiku_xpm_to_image(t_eiku_context *ctx, char **xpm_data);

EIKU_API int eiku_resize_image(t_eiku_context *ctx, t_eiku_image *img,
                               int new_width, int new_height);