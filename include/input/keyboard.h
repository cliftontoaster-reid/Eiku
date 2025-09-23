/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:51 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 01:02:34 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

#include "core/core.h"
#include "core/platform.h"

#define EIKU_KEY_COUNT 103
#define EIKU_KEY_BUFFER_SIZE 256

typedef enum e_eiku_keycode {
  EIKU_KEY_UNKNOWN = 0,

  /* Alphabet */
  EIKU_KEY_A,
  EIKU_KEY_B,
  EIKU_KEY_C,
  EIKU_KEY_D,
  EIKU_KEY_E,
  EIKU_KEY_F,
  EIKU_KEY_G,
  EIKU_KEY_H,
  EIKU_KEY_I,
  EIKU_KEY_J,
  EIKU_KEY_K,
  EIKU_KEY_L,
  EIKU_KEY_M,
  EIKU_KEY_N,
  EIKU_KEY_O,
  EIKU_KEY_P,
  EIKU_KEY_Q,
  EIKU_KEY_R,
  EIKU_KEY_S,
  EIKU_KEY_T,
  EIKU_KEY_U,
  EIKU_KEY_V,
  EIKU_KEY_W,
  EIKU_KEY_X,
  EIKU_KEY_Y,
  EIKU_KEY_Z,

  /* Numbers */
  EIKU_KEY_1,
  EIKU_KEY_2,
  EIKU_KEY_3,
  EIKU_KEY_4,
  EIKU_KEY_5,
  EIKU_KEY_6,
  EIKU_KEY_7,
  EIKU_KEY_8,
  EIKU_KEY_9,
  EIKU_KEY_0,

  /* Miscellaneous */
  EIKU_KEY_RETURN,
  EIKU_KEY_ESCAPE,
  EIKU_KEY_BACKSPACE,
  EIKU_KEY_TAB,
  EIKU_KEY_SPACE,
  EIKU_KEY_MINUS,
  EIKU_KEY_EQUALS,
  EIKU_KEY_LEFTBRACKET,
  EIKU_KEY_RIGHTBRACKET,
  EIKU_KEY_BACKSLASH,
  EIKU_KEY_SEMICOLON,
  EIKU_KEY_APOSTROPHE,
  EIKU_KEY_GRAVE,
  EIKU_KEY_COMMA,
  EIKU_KEY_PERIOD,
  EIKU_KEY_SLASH,
  EIKU_KEY_CAPSLOCK,

  /* Function keys */
  EIKU_KEY_F1,
  EIKU_KEY_F2,
  EIKU_KEY_F3,
  EIKU_KEY_F4,
  EIKU_KEY_F5,
  EIKU_KEY_F6,
  EIKU_KEY_F7,
  EIKU_KEY_F8,
  EIKU_KEY_F9,
  EIKU_KEY_F10,
  EIKU_KEY_F11,
  EIKU_KEY_F12,
  EIKU_KEY_PRINTSCREEN,
  EIKU_KEY_SCROLLLOCK,
  EIKU_KEY_PAUSE,
  EIKU_KEY_INSERT,
  EIKU_KEY_HOME,
  EIKU_KEY_PAGEUP,
  EIKU_KEY_DELETE,
  EIKU_KEY_END,
  EIKU_KEY_PAGEDOWN,
  EIKU_KEY_RIGHT,
  EIKU_KEY_LEFT,
  EIKU_KEY_DOWN,
  EIKU_KEY_UP,
  EIKU_KEY_NUMLOCK,
  EIKU_KEY_KP_DIVIDE,
  EIKU_KEY_KP_MULTIPLY,
  EIKU_KEY_KP_SUBTRACT,
  EIKU_KEY_KP_ADD,
  EIKU_KEY_KP_ENTER,
  EIKU_KEY_KP_1,
  EIKU_KEY_KP_2,
  EIKU_KEY_KP_3,
  EIKU_KEY_KP_4,
  EIKU_KEY_KP_5,
  EIKU_KEY_KP_6,
  EIKU_KEY_KP_7,
  EIKU_KEY_KP_8,
  EIKU_KEY_KP_9,
  EIKU_KEY_KP_0,
  EIKU_KEY_KP_DECIMAL,

  /* Modifier keys (auto-assigned to follow previous entries; will start at 96)
   */
  EIKU_KEY_LCTRL,
  EIKU_KEY_LSHIFT,
  EIKU_KEY_LALT,
  EIKU_KEY_LGUI,
  EIKU_KEY_RCTRL,
  EIKU_KEY_RSHIFT,
  EIKU_KEY_RALT,
  EIKU_KEY_RGUI,
} t_keycode;

/* Key State Enum: Tracks per-key status. */
typedef enum e_eiku_key_state {
  EIKU_KEY_STATE_UP = 0,   // Not pressed
  EIKU_KEY_STATE_PRESSED,  // Just pressed this frame
  EIKU_KEY_STATE_HELD,     // Held down
  EIKU_KEY_STATE_RELEASED  // Just released this frame
} t_eiku_key_state;

/* Modifier Flags: Bitmask for modifier keys. */
typedef enum e_eiku_modifier {
  EIKU_MOD_NONE = 0,
  EIKU_MOD_CTRL = 1 << 0,
  EIKU_MOD_ALT = 1 << 1,
  EIKU_MOD_SHIFT = 1 << 2,
  EIKU_MOD_GUI = 1 << 3,
  // Add more (e.g., META, SUPER) as needed
} t_eiku_modifier;

/* Keyboard State Struct: Holds all state data. */
typedef struct s_eiku_keyboard_state {
  t_eiku_context *ctx;
  // State per key
  t_eiku_key_state keys[EIKU_KEY_COUNT];
  // Current modifiers (bitmask)
  t_eiku_modifier modifiers;
  // Buffer for text input (e.g., typed characters)
  char text_buffer[EIKU_KEY_BUFFER_SIZE];
  // Length of text in buffer
  size_t text_length;
} t_eiku_keyboard_state;

/* Initialize keyboard state. Call once at startup. */
EIKU_API int eiku_keyboard_init(t_eiku_context *ctx,
                                t_eiku_keyboard_state *state);

/* Update keyboard state. Call each frame. */
EIKU_API void eiku_keyboard_update(t_eiku_keyboard_state *state, ...);

/* Check if a key is currently down (pressed or held). */
EIKU_API int eiku_key_is_down(t_eiku_keyboard_state *state, t_keycode key);

/* Check if a key was just pressed this frame. */
EIKU_API int eiku_key_is_pressed(t_eiku_keyboard_state *state, t_keycode key);

/* Check if a key was just released this frame. */
EIKU_API int eiku_key_is_released(t_eiku_keyboard_state *state, t_keycode key);

/* Ends a loop by turning pressed keys into held keys and released keys into up
 * keys. */
EIKU_API void eiku_key_end_loop(t_eiku_keyboard_state *state);

#ifdef EIKU_PLATFORM_LINUX

/* Handle an X11 event for keyboard input. Call from your X11 event loop.
 * Returns 1 if the event was handled, 0 otherwise. */
EIKU_INT int eiku_key_handle_xevent(t_eiku_keyboard_state *state,
                                    XEvent *xevent);

/* Convert an X11 KeySym to the library's internal keycode. Defined in
 * src/input/keyboard/eiku_keycode_from_xkeysym.c */
EIKU_INT t_keycode eiku_keycode_from_xkeysym(KeySym keysym);

#endif
