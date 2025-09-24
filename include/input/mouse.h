/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:49 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:35:59 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "core/core.h"
#include "core/platform.h"

typedef struct s_mouse {
  t_eiku_context* ctx;
  t_eiku_window* window;

  int32_t x;
  int32_t y;
  int32_t dx;
  int32_t dy;
  int32_t wheel_x;
  int32_t wheel_y;
  int32_t wheel_dx;
  int32_t wheel_dy;
  bool buttons[8];
  bool grabbed;
  bool hidden;
} t_mouse;

EIKU_API int mouse_init(t_mouse* mouse, t_eiku_context* ctx,
                        t_eiku_window* window);

EIKU_API int mouse_update(t_mouse* mouse, ...);

EIKU_API int mouse_set_position(t_mouse* mouse, int32_t x, int32_t y);

EIKU_API int mouse_set_grabbed(t_mouse* mouse, bool grabbed);

EIKU_API int mouse_set_hidden(t_mouse* mouse, bool hidden);

EIKU_API void mouse_reset_delta(t_mouse* mouse);

EIKU_API bool mouse_is_button_down(t_mouse* mouse, int button);

#ifdef EIKU_PLATFORM_LINUX

EIKU_INT int eiku_mouse_handle_xevent(t_mouse* mouse, XEvent* event);

EIKU_INT int eiku_mouse_move(t_mouse* mouse, int32_t x, int32_t y);

EIKU_INT int eiku_mouse_set_hidden(t_mouse* mouse, bool hidden);

#endif