/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_get_color_value.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+
        +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:36:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/22 19:36:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <stdlib.h>

EIKU_API int eiku_get_color_value(t_eiku_context *ctx, int color) {
  unsigned int r;
  unsigned int g;
  unsigned int b;
  unsigned long pixel;
  int rshift;
  int rbits;
  int gshift;
  int gbits;
  int bshift;
  int bbits;
  unsigned long alpha_mask;

  if (!ctx) return (0);
  /* color is 0xRRGGBB — convert according to visual masks and shifts */
  r = (color >> 16) & 0xFF;
  g = (color >> 8) & 0xFF;
  b = (color) & 0xFF;
  pixel = 0;
  /* Scale 8-bit components to mask bits */
  rshift = ctx->decrgb[0];
  rbits = ctx->decrgb[1];
  gshift = ctx->decrgb[2];
  gbits = ctx->decrgb[3];
  bshift = ctx->decrgb[4];
  bbits = ctx->decrgb[5];
  if (rbits > 0) pixel |= ((r >> (8 - rbits)) << rshift);
  if (gbits > 0) pixel |= ((g >> (8 - gbits)) << gshift);
  if (bbits > 0) pixel |= ((b >> (8 - bbits)) << bshift);
  /* For 32-bit visuals,
          set alpha to fully opaque if there's an alpha channel */
  if (ctx->depth == 32) {
    alpha_mask = ~(ctx->visual->red_mask | ctx->visual->green_mask |
                   ctx->visual->blue_mask);
    if (alpha_mask != 0) {
      pixel |= alpha_mask; /* Set alpha bits to opaque */
    }
  }
  return ((int)pixel);
}

#else
EIKU_API int eiku_get_color_value(t_eiku_context *ctx, int color) {
  (void)ctx;
  (void)color;
  return (0);
}
#endif