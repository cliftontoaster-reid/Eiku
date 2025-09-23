/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_new_image.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:44:15 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/22 23:22:53 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* helper prototypes (file scope) */
static t_eiku_image *create_shm_image(t_eiku_context *ctx, t_eiku_image *img,
                                      int width, int height);
static t_eiku_image *create_ximage(t_eiku_context *ctx, t_eiku_image *img,
                                   int width, int height);

/**
 * @brief Create a new image in the given context
 *
 * This function creates a new image with the specified width and height
 * in the provided Eiku context. The image is created using X11's XCreateImage
 * function and is stored in a t_eiku_image structure.
 *
 * @param ctx Pointer to the Eiku context
 * @param width Width of the image in pixels
 * @param height Height of the image in pixels
 * @return Pointer to the newly created t_eiku_image, or NULL on failure
 */
EIKU_API t_eiku_image *eiku_new_image(t_eiku_context *ctx, int width,
                                      int height) {
  t_eiku_image *img;

  // Validate parameters
  if (ctx == NULL || width <= 0 || height <= 0) return (NULL);
  // Allocate memory for the image structure
  img = malloc(sizeof(t_eiku_image));
  if (img == NULL) return (NULL);
  memset(img, 0, sizeof(t_eiku_image));
  if (ctx->use_xshm) {
    if (!create_shm_image(ctx, img, width, height)) {
      free(img);
      return (NULL);
    }
  } else {
    if (!create_ximage(ctx, img, width, height)) {
      free(img);
      return (NULL);
    }
  }
  return (img);
}

static t_eiku_image *create_shm_image(t_eiku_context *ctx, t_eiku_image *img,
                                      int width, int height) {
  // Create an XImage using shared memory
  img->image = XShmCreateImage(ctx->display, ctx->visual, ctx->depth, ZPixmap,
                               NULL, &img->shm, width, height);
  if (img->image == NULL) {
    return (NULL);
  }
  // Allocate shared memory segment
  img->shm.shmid =
      shmget(IPC_PRIVATE, img->image->height * img->image->bytes_per_line,
             IPC_CREAT | 0777);
  if (img->shm.shmid < 0) {
    XDestroyImage(img->image);
    return (NULL);
  }
  // Attach the shared memory segment
  img->shm.shmaddr = shmat(img->shm.shmid, 0, 0);
  if (img->shm.shmaddr == (char *)-1) {
    shmctl(img->shm.shmid, IPC_RMID, 0);
    XDestroyImage(img->image);
    return (NULL);
  }
  // Set image data pointer to shared memory
  img->image->data = img->shm.shmaddr;
  img->shm.readOnly = False;
  // Attach to X server
  if (!XShmAttach(ctx->display, &img->shm)) {
    shmdt(img->shm.shmaddr);
    shmctl(img->shm.shmid, IPC_RMID, 0);
    XDestroyImage(img->image);
    return (NULL);
  }
  // Add XSync to ensure server has processed the attachment
  XSync(ctx->display, False);
  // Initialize image properties
  img->width = width;
  img->height = height;
  img->bpp = img->image->bits_per_pixel;
  img->size_line = img->image->bytes_per_line;
  img->data = img->image->data;
  img->format = ZPixmap;
  // Initialize graphics context and pixmap
  img->pix = XCreatePixmap(ctx->display, ctx->root, width, height, ctx->depth);
  if (img->pix == 0) {
    XShmDetach(ctx->display, &img->shm);
    shmdt(img->shm.shmaddr);
    shmctl(img->shm.shmid, IPC_RMID, 0);
    XDestroyImage(img->image);
    return (NULL);
  }
  img->gc = XCreateGC(ctx->display, img->pix, 0, NULL);
  if (img->gc == NULL) {
    XFreePixmap(ctx->display, img->pix);
    XShmDetach(ctx->display, &img->shm);
    shmdt(img->shm.shmaddr);
    shmctl(img->shm.shmid, IPC_RMID, 0);
    XDestroyImage(img->image);
    return (NULL);
  }
  img->type = EIKU_TYPE_SHM;
  return (img);
}

static t_eiku_image *create_ximage(t_eiku_context *ctx, t_eiku_image *img,
                                   int width, int height) {
  // Create a standard XImage
  img->image = XCreateImage(ctx->display, ctx->visual, ctx->depth, ZPixmap, 0,
                            NULL, width, height, 32, 0);
  if (img->image == NULL) {
    return (NULL);
  }
  // Allocate memory for the image data
  img->image->data = malloc(img->image->height * img->image->bytes_per_line);
  if (img->image->data == NULL) {
    XDestroyImage(img->image);
    return (NULL);
  }
  memset(img->image->data, 0, img->image->height * img->image->bytes_per_line);
  // Ensure byte order matches the display
  img->image->byte_order = ImageByteOrder(ctx->display);
  img->image->bitmap_bit_order = BitmapBitOrder(ctx->display);
  img->data = img->image->data;
  img->width = width;
  img->height = height;
  img->bpp = img->image->bits_per_pixel;
  img->size_line = img->image->bytes_per_line;
  img->format = ZPixmap;
  // Initialize graphics context and pixmap
  img->pix = XCreatePixmap(ctx->display, ctx->root, width, height, ctx->depth);
  if (img->pix == 0) {
    XDestroyImage(img->image);
    return (NULL);
  }
  img->gc = XCreateGC(ctx->display, img->pix, 0, NULL);
  if (img->gc == NULL) {
    XFreePixmap(ctx->display, img->pix);
    XDestroyImage(img->image);
    return (NULL);
  }
  img->type = EIKU_TYPE_XIMAGE;
  // Clear shared memory info since not using XShm
  memset(&img->shm, 0, sizeof(img->shm));
  return (img);
}
#else
#error "Eiku library is only supported on Linux platforms."
#endif /* EIKU_PLATFORM_LINUX */