/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_key_is_pressed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:59:58 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 02:19:16 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API int eiku_key_is_pressed(t_eiku_keyboard_state *state, t_keycode key) {
  if (!state) return 0;

  /* Ensure keycode is within our key array bounds. */
  if (key <= 0 || key >= EIKU_KEY_COUNT) return 0;

  return state->keys[key - 1] == EIKU_KEY_STATE_PRESSED;
}