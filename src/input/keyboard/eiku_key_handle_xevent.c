/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_key_handle_xevent.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:39:05 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 00:55:42 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

EIKU_INT int eiku_key_handle_xevent(t_eiku_keyboard_state *state,
                                    XEvent *xevent) {
  if (!state || !xevent) return 0;

  if (xevent->type != KeyPress && xevent->type != KeyRelease)
    return 0;  // Not a key event

  KeySym keysym = XLookupKeysym(&xevent->xkey, 0);
  t_keycode keycode = eiku_keycode_from_xkeysym(keysym);
  if (keycode == EIKU_KEY_UNKNOWN) return 0;  // Unrecognized key

  /* Ensure keycode is within our key array bounds. */
  if ((unsigned)keycode >= (unsigned)EIKU_KEY_COUNT) return 0;

  if (xevent->type == KeyPress) {
    /* If the key was already held, do nothing; otherwise mark it pressed. */
    if (state->keys[keycode - 1] == EIKU_KEY_STATE_HELD) return 1;
    state->keys[keycode - 1] = EIKU_KEY_STATE_PRESSED;
  } else {
    state->keys[keycode - 1] = EIKU_KEY_STATE_RELEASED;
  }

  return 1; /* Event handled */
}

#endif