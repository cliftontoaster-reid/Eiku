/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_int_deal_shm.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 00:10:32 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/22 00:15:05 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

EIKU_INT int eiku_int_deal_shm(t_eiku_context *ctx) {
  int major;
  int minor;
  int present;
  char hostbuf[256];
  char *dpy;

  if (!ctx || !ctx->display) return (EIKU_ERROR_DISPLAY_FAILED);
  ctx->use_xshm = false;
  /* Query XShm support */
  ctx->use_xshm = XShmQueryVersion(ctx->display, &major, &minor, &present);
  if (!ctx->use_xshm || !present) {
    ctx->use_xshm = false;
    ctx->pshm_format = -1;
    return (EIKU_SUCCESS);
  }
  /* Get X server pixmap format for shared memory */
  ctx->pshm_format = XShmPixmapFormat(ctx->display);
  /* Disable SHM if DISPLAY is remote */
  if (gethostname(hostbuf, sizeof(hostbuf)) == 0) {
    dpy = getenv("DISPLAY");
    if (dpy && *dpy && *dpy != ':' && strncmp(dpy, hostbuf, strlen(hostbuf)) &&
        strncmp(dpy, "localhost", 9)) {
      ctx->pshm_format = -1;
      ctx->use_xshm = false;
    }
  }
  return (EIKU_SUCCESS);
}
#endif