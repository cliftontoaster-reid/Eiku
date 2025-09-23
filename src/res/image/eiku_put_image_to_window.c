/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_put_image_to_window.c                         :+:      :+:    :+:   */
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
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/extensions/XShm.h>
# include <stdlib.h>

EIKU_API int eiku_put_image_to_window(t_eiku_context *ctx, t_eiku_window *win,
	t_eiku_image *img, int x, int y)
{
	if (!ctx || !win || !img)
		return (EIKU_ERROR_INVALID_PARAM);
	if (!ctx->display)
		return (EIKU_ERROR_DISPLAY_FAILED);
	// Put image directly to window - avoid pixmap intermediate step
	if (img->type == EIKU_TYPE_SHM)
	{
		XShmPutImage(ctx->display, win->window, win->gc, img->image, 0, 0, x, y,
			img->width, img->height, False);
	}
	else
	{
		XPutImage(ctx->display, win->window, win->gc, img->image, 0, 0, x, y,
			img->width, img->height);
	}
	if (ctx->do_flush)
		XFlush(ctx->display);
	return (EIKU_SUCCESS);
}

EIKU_API int eiku_get_color_value(t_eiku_context *ctx, int color)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;
	unsigned long	pixel;
	int				rshift;
	int				rbits;
	int				gshift;
	int				gbits;
	int				bshift;
	int				bbits;

	if (!ctx)
		return (0);
	/* color is 0xRRGGBB — convert according to visual masks and shifts */
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color)&0xFF;
	pixel = 0;
	/* Scale 8-bit components to mask bits */
	rshift = ctx->decrgb[0];
	rbits = ctx->decrgb[1];
	gshift = ctx->decrgb[2];
	gbits = ctx->decrgb[3];
	bshift = ctx->decrgb[4];
	bbits = ctx->decrgb[5];
	if (rbits > 0)
		pixel |= ((r >> (8 - rbits)) << rshift);
	if (gbits > 0)
		pixel |= ((g >> (8 - gbits)) << gshift);
	if (bbits > 0)
		pixel |= ((b >> (8 - bbits)) << bshift);
	return ((int)pixel);
}

#else
EIKU_API int eiku_put_image_to_window(t_eiku_context *ctx, t_eiku_window *win,
	t_eiku_image *img, int x, int y)
{
	(void)ctx;
	(void)win;
	(void)img;
	(void)x;
	(void)y;
	return (EIKU_ERROR_INVALID_PARAM);
}

EIKU_API int eiku_get_color_value(t_eiku_context *ctx, int color)
{
	(void)ctx;
	(void)color;
	return (0);
}
#endif
