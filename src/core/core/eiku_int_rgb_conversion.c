/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_int_rgb_conversion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 00:18:48 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/03 15:48:15 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX

static inline void compute_red_shift_bits(unsigned long rm, int *r_shift,
                                          int *r_bits) {
  if (rm == 0) {
    *r_shift = 0;
    *r_bits = 0;
    return;
  }
  while (!(rm & 1UL)) {
    rm >>= 1;
    (*r_shift)++;
  }
  while (rm & 1UL) {
    rm >>= 1;
    (*r_bits)++;
  }
}

static inline void compute_green_shift_bits(unsigned long gm, int *g_shift,
                                            int *g_bits) {
  if (gm == 0) {
    *g_shift = 0;
    *g_bits = 0;
    return;
  }
  while (!(gm & 1UL)) {
    gm >>= 1;
    (*g_shift)++;
  }
  while (gm & 1UL) {
    gm >>= 1;
    (*g_bits)++;
  }
}

static inline void compute_blue_shift_bits(unsigned long bm, int *b_shift,
                                           int *b_bits) {
  if (bm == 0) {
    *b_shift = 0;
    *b_bits = 0;
    return;
  }
  while (!(bm & 1UL)) {
    bm >>= 1;
    (*b_shift)++;
  }
  while (bm & 1UL) {
    bm >>= 1;
    (*b_bits)++;
  }
}

EIKU_INT int eiku_int_rgb_conversion(t_eiku_context *ctx) {
  unsigned long rm;
  unsigned long gm;
  unsigned long bm;
  int i;

  if (!ctx || !ctx->visual) return (EIKU_ERROR_INVALID_PARAM);
  /* Read masks from the Visual provided by X11 */
  rm = ctx->visual->red_mask;
  gm = ctx->visual->green_mask;
  bm = ctx->visual->blue_mask;
  /* zero the decrgb array: [r_shift, r_bits, g_shift, g_bits, b_shift,
          b_bits] */
  for (i = 0; i < 6; ++i) ctx->decrgb[i] = 0;
  /* compute red shift and bit count */
  compute_red_shift_bits(rm, &ctx->decrgb[0], &ctx->decrgb[1]);
  /* compute green shift and bit count */
  compute_green_shift_bits(gm, &ctx->decrgb[2], &ctx->decrgb[3]);
  /* compute blue shift and bit count */
  compute_blue_shift_bits(bm, &ctx->decrgb[4], &ctx->decrgb[5]);
  return (EIKU_SUCCESS);
}

#endif