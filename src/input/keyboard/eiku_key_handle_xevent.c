/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_key_handle_xevent.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:39:05 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 02:19:08 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <string.h>

static EIKU_INT int eiku_keycode_to_modifier(t_keycode keycode) {
  switch (keycode) {
    case EIKU_KEY_LCTRL:
    case EIKU_KEY_RCTRL:
      return EIKU_MOD_CTRL;
    case EIKU_KEY_LSHIFT:
    case EIKU_KEY_RSHIFT:
      return EIKU_MOD_SHIFT;
    case EIKU_KEY_LALT:
    case EIKU_KEY_RALT:
      return EIKU_MOD_ALT;
    case EIKU_KEY_LGUI:
    case EIKU_KEY_RGUI:
      return EIKU_MOD_GUI;
    default:
      return 0;
  }
}

EIKU_INT int eiku_key_handle_xevent(t_eiku_keyboard_state *state,
                                    XEvent *xevent) {
  if (!state || !xevent) return 0;

  if (xevent->type != KeyPress && xevent->type != KeyRelease)
    return 0;  // Not a key event

  KeySym keysym = XLookupKeysym(&xevent->xkey, 0);
  t_keycode keycode = eiku_keycode_from_xkeysym(keysym);
  if (keycode == EIKU_KEY_UNKNOWN ||
      (unsigned)keycode >= (unsigned)EIKU_KEY_COUNT)
    return 0;  // Unrecognized or out-of-bounds key

  if (xevent->type == KeyPress) {
    /* If repeats are disabled and the key was already held, do nothing;
     * otherwise mark it pressed. */
    if (!state->enable_key_repeats &&
        state->keys[keycode - 1] == EIKU_KEY_STATE_HELD)
      return 1;
    state->keys[keycode - 1] = EIKU_KEY_STATE_PRESSED;

    if (keycode >= EIKU_KEY_LCTRL) {
      // Modifier key pressed
      state->modifiers |= eiku_keycode_to_modifier(keycode);
    } else {
      // Regular key pressed - handle text input
      char buffer[32];
      int len =
          XLookupString(&xevent->xkey, buffer, sizeof(buffer), NULL, NULL);
      if (len > 0) {
        // Append to text buffer if space allows
        if (state->text_length + len < EIKU_KEY_BUFFER_SIZE) {
          memcpy(state->text_buffer + state->text_length, buffer, len);
          state->text_length += len;
          state->text_buffer[state->text_length] = '\0';  // Null-terminate
        }
      }
    }
  } else {
    state->keys[keycode - 1] = EIKU_KEY_STATE_RELEASED;

    if (keycode >= EIKU_KEY_LCTRL) {
      // Modifier key released
      state->modifiers &= ~eiku_keycode_to_modifier(keycode);
    }
  }

  return 1; /* Event handled */
}

#endif