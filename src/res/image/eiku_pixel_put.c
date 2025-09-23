/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_pixel_put.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:17:35 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:06:37 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"
#include <stdint.h>
#include <string.h>

#ifdef EIKU_PLATFORM_LINUX
# include <X11/Xlib.h>
# include <stdlib.h>

// Convert integer color (0xRRGGBB) to byte sequence based on bits-per-pixel
static int	color_to_bytes(int color, int bpp, uint8_t *out)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	int			bytes;
	uint64_t	pixel;
	uint16_t	r5;
	uint16_t	g6;
	uint16_t	b5;
	uint16_t	val;
	int			bits_per_channel;
	uint8_t		shift;
	uint8_t		mask;
	uint64_t	scaled_r;
	uint64_t	scaled_g;
	uint64_t	scaled_b;
	uint64_t	scaled_a;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	uint8_t a = 0xFF; // Default alpha to opaque
	bytes = bpp / 8;
	pixel = 0;
	// Handle common cases with specific layouts
	if (bpp == 32)
	{
		// BGRA layout (most common 32-bit)
		out[0] = b;
		out[1] = g;
		out[2] = r;
		out[3] = a;
	}
	else if (bpp == 24)
	{
		// BGR layout
		out[0] = b;
		out[1] = g;
		out[2] = r;
	}
	else if (bpp == 16)
	{
		// RGB565 layout
		r5 = r >> 3;
		g6 = g >> 2;
		b5 = b >> 3;
		val = (r5 << 11) | (g6 << 5) | b5;
		out[0] = val & 0xFF;
		out[1] = (val >> 8) & 0xFF;
	}
	else if (bpp == 8)
	{
		// Grayscale approximation: 0.299*R + 0.587*G + 0.114*B
		out[0] = (uint8_t)((77 * r + 150 * g + 29 * b) >> 8);
	}
	else if (bpp > 0 && bpp <= 64 && (bpp % 8) == 0)
	{
		// Generic handling: pack RGBA into available bits
		bits_per_channel = bpp / 4;
		if (bits_per_channel > 8)
			bits_per_channel = 8;
		shift = 8 - bits_per_channel;
		mask = (1 << bits_per_channel) - 1;
		scaled_r = (r >> shift) & mask;
		scaled_g = (g >> shift) & mask;
		scaled_b = (b >> shift) & mask;
		scaled_a = (a >> shift) & mask;
		pixel = (scaled_r << (3 * bits_per_channel)) | (scaled_g << (2
					* bits_per_channel)) | (scaled_b << bits_per_channel) | scaled_a;
		// Pack into bytes (little-endian)
		for (int i = 0; i < bytes; i++)
		{
			out[i] = (pixel >> (8 * i)) & 0xFF;
		}
	}
	else
	{
		return (EIKU_ERROR_INVALID_PARAM);
	}
	return (EIKU_SUCCESS);
}

// Helper: put pixel into image only
EIKU_API int eiku_image_pixel_put(t_eiku_image *img, int x, int y, int color)
{
	uint8_t	*ptr;
	int		bytes;

	uint8_t pixel_bytes[8]; // Support up to 64-bit pixels
	if (!img || x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (EIKU_ERROR_INVALID_PARAM);
	bytes = img->bpp / 8;
	if (color_to_bytes(color, img->bpp, pixel_bytes) != EIKU_SUCCESS)
		return (EIKU_ERROR_INVALID_PARAM);
	ptr = (uint8_t *)img->data + y * img->size_line + x * bytes;
	memcpy(ptr, pixel_bytes, bytes);
	return (EIKU_SUCCESS);
}

EIKU_API int eiku_pixel_put(t_eiku_context *ctx, t_eiku_window *win, int x,
	int y, int color)
{
	unsigned long	px;

	if (!ctx || !win)
		return (EIKU_ERROR_INVALID_PARAM);
# if defined(EIKU_PLATFORM_LINUX)
	if (!ctx->display || !win->window || !win->gc)
		return (EIKU_ERROR_WINDOW_FAILED);
	/* Convert color to pixel value using context conversion */
	px = (unsigned long)eiku_get_color_value(ctx, color);
	XSetForeground(ctx->display, win->gc, px);
	XDrawPoint(ctx->display, win->window, win->gc, x, y);
	if (ctx->do_flush)
		XFlush(ctx->display);
	return (EIKU_SUCCESS);
# else
	(void)ctx;
	(void)win;
	(void)x;
	(void)y;
	(void)color;
	return (EIKU_ERROR_INVALID_PARAM);
# endif
}

#endif
