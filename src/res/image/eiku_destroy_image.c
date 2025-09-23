/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_destroy_image.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+
        +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:30:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/22 19:30:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <stdlib.h>
#include <sys/shm.h>

EIKU_API int eiku_destroy_image(t_eiku_context *ctx, t_eiku_image *img) {
  if (!ctx || !img) return (EIKU_ERROR_INVALID_PARAM);
  if (ctx->display) {
    if (img->gc) XFreeGC(ctx->display, img->gc);
    if (img->pix) XFreePixmap(ctx->display, img->pix);
    if (img->image) {
      if (img->type == EIKU_TYPE_SHM) {
        XShmDetach(ctx->display, &img->shm);
        if (img->shm.shmaddr) shmdt(img->shm.shmaddr);
        if (img->shm.shmid >= 0) shmctl(img->shm.shmid, IPC_RMID, 0);
        XDestroyImage(img->image);
        /* destroys header but not shm data */
      } else {
        /* XDestroyImage will free image->data when allocated via XCreateImage
         * for non-shm we allocated data manually and XDestroyImage frees it */
        XDestroyImage(img->image);
      }
    }
  }
  free(img);
  return (EIKU_SUCCESS);
}

#else
EIKU_API int eiku_destroy_image(t_eiku_context *ctx, t_eiku_image *img) {
  (void)ctx;
  (void)img;
  return (EIKU_ERROR_INVALID_PARAM);
}
#endif
