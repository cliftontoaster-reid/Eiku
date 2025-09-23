/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_new_window.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:51:45 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 14:51:58 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

EIKU_API bool eiku_window_set_title(t_eiku_context *ctx, t_eiku_window *win,
                                    const char *title) {
#ifdef EIKU_PLATFORM_LINUX
  if (!ctx || !win || !title) {
    return (false);
  }
  XStoreName(ctx->display, win->window, title);
#else
  (void)ctx;
  (void)win;
  (void)title;
  return (false);
#endif
  win->title = strdup(title);
  return (win->title != NULL);
}

EIKU_API t_eiku_window *eiku_new_window(t_eiku_context *ctx, int w, int h,
                                        const char *t) {
  t_eiku_window *new_win;

  if (!ctx || w <= 0 || h <= 0 || !t) {
    return (NULL);
  }
#ifdef EIKU_PLATFORM_LINUX
  new_win = calloc(1, sizeof(t_eiku_window));
  if (!new_win) {
    return (NULL);
  }
  new_win->width = w;
  new_win->height = h;
  new_win->title = strdup(t);

  // Create window with the same visual as the context to avoid depth mismatches
  XSetWindowAttributes attrs;
  attrs.border_pixel = BlackPixel(ctx->display, ctx->screen);
  attrs.background_pixel = WhitePixel(ctx->display, ctx->screen);
  attrs.colormap = ctx->cmap;

  new_win->window = XCreateWindow(
      ctx->display, ctx->root, 0, 0, w, h, 0, ctx->depth, InputOutput,
      ctx->visual, CWBorderPixel | CWBackPixel | CWColormap, &attrs);

  if (!new_win->window) {
    free(new_win->title);
    free(new_win);
    return (NULL);
  }
  new_win->gc = XCreateGC(ctx->display, new_win->window, 0, NULL);
  if (!new_win->gc) {
    XDestroyWindow(ctx->display, new_win->window);
    free(new_win->title);
    free(new_win);
    return (NULL);
  }
  eiku_window_set_title(ctx, new_win, t);
  XSelectInput(ctx->display, new_win->window,
               ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                   ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
  XMapWindow(ctx->display, new_win->window);
  new_win->next = ctx->win_list;
  ctx->win_list = new_win;
  return (new_win);
#else
  return (NULL);
#endif
}