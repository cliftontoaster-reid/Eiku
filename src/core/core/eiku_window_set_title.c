/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_window_set_title.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:51:45 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/03 15:48:14 by lfiorell@st      ###   ########.fr       */
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