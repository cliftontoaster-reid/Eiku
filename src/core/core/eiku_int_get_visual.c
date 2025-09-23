/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_int_get_visual.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 23:13:48 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/22 00:08:19 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX

static inline int try_preferred_visual(t_eiku_context *ctx, int screen) {
  XVisualInfo vinfo;

  if (XMatchVisualInfo(ctx->display, screen, 32, TrueColor, &vinfo) ||
      XMatchVisualInfo(ctx->display, screen, 24, TrueColor, &vinfo)) {
    ctx->visual = vinfo.visual;
    ctx->depth = vinfo.depth;
    ctx->private_cmap = 0;
    return (EIKU_SUCCESS);
  }
  return (EIKU_ERROR_VISUAL_FAILED);
}

static inline int try_fallback_visual(t_eiku_context *ctx, int screen) {
  XVisualInfo vreq;
  XVisualInfo *vlist;
  int nitems;
  int best;

  vreq.screen = screen;
  vreq.class = TrueColor;
  vlist = XGetVisualInfo(ctx->display, VisualScreenMask | VisualClassMask,
                         &vreq, &nitems);
  if (vlist && nitems > 0) {
    best = 0;
    for (int i = 1; i < nitems; ++i) {
      if (vlist[i].depth > vlist[best].depth) best = i;
    }
    ctx->visual = vlist[best].visual;
    ctx->depth = vlist[best].depth;
    ctx->private_cmap = 0;
    XFree(vlist);
    return (EIKU_SUCCESS);
  }
  if (vlist) XFree(vlist);
  return (EIKU_ERROR_VISUAL_FAILED);
}

// Get the best visual for the current display and screen
int eiku_int_get_visual(t_eiku_context *ctx) {
  int screen;

  if (!ctx || !ctx->display) return (EIKU_ERROR_DISPLAY_FAILED);
  screen = DefaultScreen(ctx->display);
  if (try_preferred_visual(ctx, screen) == EIKU_SUCCESS) return (EIKU_SUCCESS);
  return (try_fallback_visual(ctx, screen));
}

#endif