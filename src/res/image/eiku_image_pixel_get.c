/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_image_pixel_get.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:33:48 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/03 15:48:13 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <stdlib.h>

static int bytes_to_color(int bpp, const uint8_t *in) {
  if (!in || bpp <= 0 || (bpp % 8) != 0)
    return 0;  // invalid -> return black as safe default

  if (bpp == 32) {
    // Stored as [B, G, R, A]
    uint8_t b = in[0];
    uint8_t g = in[1];
    uint8_t r = in[2];
    return (r << 16) | (g << 8) | b;
  } else if (bpp == 24) {
    // Stored as [B, G, R]
    uint8_t b = in[0];
    uint8_t g = in[1];
    uint8_t r = in[2];
    return (r << 16) | (g << 8) | b;
  } else if (bpp == 16) {
    // RGB565 little-endian: 5 bits R, 6 bits G, 5 bits B
    uint16_t val = (uint16_t)in[0] | ((uint16_t)in[1] << 8);
    uint8_t r5 = (val >> 11) & 0x1F;
    uint8_t g6 = (val >> 5) & 0x3F;
    uint8_t b5 = val & 0x1F;
    // Expand to 8-bit with replication
    uint8_t r = (r5 << 3) | (r5 >> 2);
    uint8_t g = (g6 << 2) | (g6 >> 4);
    uint8_t b = (b5 << 3) | (b5 >> 2);
    return (r << 16) | (g << 8) | b;
  } else if (bpp == 8) {
    // Grayscale approximation stored in single byte
    uint8_t v = in[0];
    return (v << 16) | (v << 8) | v;
  } else {
    // Generic handling for bpp % 8 == 0 (packed RGBA-like ordering used
    // in color_to_bytes)
    int bytes = bpp / 8;
    uint64_t pixel = 0;
    for (int i = 0; i < bytes; ++i)
      pixel |= ((uint64_t)in[i]) << (8 * i);  // little-endian

    int bits_per_channel = bpp / 4;
    if (bits_per_channel > 8) bits_per_channel = 8;
    if (bits_per_channel <= 0) return 0;

    uint64_t mask = ((uint64_t)1 << bits_per_channel) - 1;
    uint64_t scaled_r = (pixel >> (3 * bits_per_channel)) & mask;
    uint64_t scaled_g = (pixel >> (2 * bits_per_channel)) & mask;
    uint64_t scaled_b = (pixel >> (1 * bits_per_channel)) & mask;

    // Scale channels to 8-bit: val * 255 / max
    uint32_t r = (uint32_t)((scaled_r * 255 + (mask / 2)) / mask);
    uint32_t g = (uint32_t)((scaled_g * 255 + (mask / 2)) / mask);
    uint32_t b = (uint32_t)((scaled_b * 255 + (mask / 2)) / mask);
    return (r << 16) | (g << 8) | b;
  }
}

// Helper: get pixel from image only
EIKU_API int eiku_image_pixel_get(t_eiku_image *img, int x, int y) {
  uint8_t *ptr;
  int bytes;

  if (!img || x < 0 || y < 0 || x >= img->width || y >= img->height)
    return (EIKU_ERROR_INVALID_PARAM);
  bytes = img->bpp / 8;
  ptr = (uint8_t *)img->data + y * img->size_line + x * bytes;
  return bytes_to_color(img->bpp, ptr);
}

#endif