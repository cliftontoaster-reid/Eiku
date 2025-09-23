/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_keyboard_update.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 01:00:51 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 01:03:40 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xlib.h>
#include <stdarg.h>

#include "input/keyboard.h"

EIKU_API void eiku_keyboard_update(t_eiku_keyboard_state *state, ...) {
  if (!state) return;

#ifdef EIKU_PLATFORM_LINUX
  // First va arg is the XEvent pointer
  va_list args;
  va_start(args, state);
  XEvent *xevent = va_arg(args, XEvent *);
  va_end(args);

  if (xevent) {
    // Handle the XEvent for keyboard input
    eiku_key_handle_xevent(state, xevent);
  }
#endif
}
