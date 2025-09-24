/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_key_is_down.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:59:35 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 00:59:50 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API int eiku_key_is_down(t_eiku_keyboard_state *state, t_keycode key) {
  if (!state) return 0;

  /* Ensure keycode is within our key array bounds. */
  if ((unsigned)key >= (unsigned)EIKU_KEY_COUNT) return 0;

  t_eiku_key_state kstate = state->keys[key - 1];
  return (kstate == EIKU_KEY_STATE_PRESSED || kstate == EIKU_KEY_STATE_HELD);
}