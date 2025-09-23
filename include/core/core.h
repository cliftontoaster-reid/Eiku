/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:57 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/22 18:26:17 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** EIKU Core - X11 Driver
**
** This X11 driver implementation is inspired by the MiniLibX (MLX) library
** originally developed by Olivier Crouzet and contributors at 42 School.
**
** While the API design and concepts are inspired by MiniLibX, this is an
** original implementation written from scratch for the Eiku game engine.
** This code is licensed under the GNU Lesser General Public License v3.0
** (see LICENCE.md in the project root).
**
** Original MiniLibX: https://github.com/42Paris/minilibx-linux
** MiniLibX License: Custom license by 42 School
**
** Acknowledgments:
** - The MiniLibX team for the excellent API design and X11 integration patterns
** - The 42 School community for the foundational graphics programming concepts
*/

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "res/image.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

// Forward declarations
typedef struct s_eiku_context t_eiku_context;
typedef struct s_eiku_window t_eiku_window;

// Event callback types
typedef int (*t_event_hook)(void *param);
typedef int (*t_key_hook)(int keycode, void *param);
typedef int (*t_mouse_hook)(int button, int x, int y, void *param);
typedef int (*t_expose_hook)(void *param);
typedef int (*t_loop_hook)(void *param);

// Core error codes
typedef enum e_eiku_error {
  EIKU_SUCCESS = 0,
  EIKU_ERROR_INIT_FAILED = -1,
  EIKU_ERROR_DISPLAY_FAILED = -2,
  EIKU_ERROR_MEMORY_FAILED = -3,
  EIKU_ERROR_VISUAL_FAILED = -4,
  EIKU_ERROR_WINDOW_FAILED = -5,
  EIKU_ERROR_IMAGE_FAILED = -6,
  EIKU_ERROR_INVALID_PARAM = -7
} t_eiku_error;

#ifdef EIKU_PLATFORM_LINUX

// Constants for X11 driver
#define EIKU_MAX_EVENT LASTEvent
#define EIKU_TYPE_SHM_PIXMAP 3
#define EIKU_TYPE_SHM 2
#define EIKU_TYPE_XIMAGE 1

// Event list structure for hook management
typedef struct s_event_list {
  int mask;
  t_event_hook hook;
  void *param;
} t_event_list;

// Window list structure - linked list of windows
typedef struct s_eiku_window {
  Window window;
  GC gc;
  struct s_eiku_window *next;

  // Event hooks
  t_mouse_hook mouse_hook;
  t_key_hook key_hook;
  t_expose_hook expose_hook;

  // Hook parameters
  void *mouse_param;
  void *key_param;
  void *expose_param;

  // Generic event hooks array
  t_event_list hooks[EIKU_MAX_EVENT];

  // Window properties
  int width;
  int height;
  char *title;
} t_eiku_window;

// Main context structure - equivalent to MiniLibX's t_xvar
typedef struct s_eiku_context {
  Display *display;
  Window root;
  int screen;
  int depth;
  Visual *visual;
  Colormap cmap;
  int private_cmap;

  // Window management
  t_eiku_window *win_list;

  // Main loop
  t_loop_hook loop_hook;
  void *loop_param;
  bool end_loop;

  // X11 shared memory extension
  bool use_xshm;
  int pshm_format;

  // Rendering
  bool do_flush;
  int decrgb[6];  // RGB conversion parameters

  // Window manager atoms
  Atom wm_delete_window;
  Atom wm_protocols;
} t_eiku_context;

#endif /* EIKU_PLATFORM_LINUX */

// Core initialization and cleanup functions
EIKU_API t_eiku_context *eiku_init(void);
EIKU_API int eiku_destroy(t_eiku_context *ctx);

// Display and screen functions
EIKU_API int eiku_get_screen_size(t_eiku_context *ctx, int *width, int *height);
EIKU_API int eiku_sync(t_eiku_context *ctx);
EIKU_API int eiku_flush(t_eiku_context *ctx);

// Window management functions
EIKU_API t_eiku_window *eiku_new_window(t_eiku_context *ctx, int width,
                                        int height, const char *title);
EIKU_API int eiku_destroy_window(t_eiku_context *ctx, t_eiku_window *win);
EIKU_API int eiku_clear_window(t_eiku_context *ctx, t_eiku_window *win);

// Window property functions
EIKU_API bool eiku_window_set_title(t_eiku_context *ctx, t_eiku_window *win,
                                    const char *title);
EIKU_API int eiku_set_window_title(t_eiku_window *win, const char *title);
EIKU_API int eiku_get_window_size(t_eiku_window *win, int *width, int *height);
EIKU_API int eiku_resize_window(t_eiku_window *win, int width, int height);

// Event handling functions
EIKU_API int eiku_string_put(t_eiku_context *ctx, t_eiku_window *win, int x,
                             int y, int color, const char *string);
EIKU_API int eiku_set_font(t_eiku_context *ctx, t_eiku_window *win,
                           const char *name);

// Event handling functions
EIKU_API int eiku_mouse_hook(t_eiku_window *win, t_mouse_hook hook,
                             void *param);
EIKU_API int eiku_key_hook(t_eiku_window *win, t_key_hook hook, void *param);
EIKU_API int eiku_expose_hook(t_eiku_window *win, t_expose_hook hook,
                              void *param);

// Generic hook system for all X11 events
EIKU_API int eiku_hook(t_eiku_window *win, int x_event, int x_mask,
                       t_event_hook hook, void *param);

// Main loop functions
EIKU_API int eiku_loop_hook(t_eiku_context *ctx, t_loop_hook hook, void *param);
EIKU_API int eiku_loop(t_eiku_context *ctx);
EIKU_API int eiku_loop_end(t_eiku_context *ctx);

// Mouse functions
EIKU_API int eiku_mouse_get_pos(t_eiku_context *ctx, t_eiku_window *win, int *x,
                                int *y);
EIKU_API int eiku_mouse_move(t_eiku_context *ctx, t_eiku_window *win, int x,
                             int y);
EIKU_API int eiku_mouse_hide(t_eiku_context *ctx, t_eiku_window *win);
EIKU_API int eiku_mouse_show(t_eiku_context *ctx, t_eiku_window *win);

// Keyboard functions
EIKU_API int eiku_key_autorepeat_off(t_eiku_context *ctx);
EIKU_API int eiku_key_autorepeat_on(t_eiku_context *ctx);

// Event utility functions
EIKU_API int eiku_flush_events(t_eiku_context *ctx);

#ifdef EIKU_PLATFORM_LINUX
// Internal X11 functions (equivalent to mlx_int_* functions)
EIKU_INT int eiku_int_get_visual(t_eiku_context *ctx);
EIKU_INT int eiku_int_deal_shm(t_eiku_context *ctx);
EIKU_INT int eiku_int_rgb_conversion(t_eiku_context *ctx);
EIKU_INT int eiku_int_set_win_event_mask(t_eiku_context *ctx,
                                         t_eiku_window *win);
EIKU_INT int eiku_int_wait_first_expose(t_eiku_context *ctx,
                                        t_eiku_window *win);
#endif /* EIKU_PLATFORM_LINUX */
