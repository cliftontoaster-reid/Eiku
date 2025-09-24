/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_key_end_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:48:28 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 00:48:45 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API void eiku_key_end_loop(t_eiku_keyboard_state *state) {
  if (!state) return;

  for (size_t i = 0; i < EIKU_KEY_COUNT; ++i) {
    if (state->keys[i] == EIKU_KEY_STATE_PRESSED) {
      state->keys[i] = EIKU_KEY_STATE_HELD;
    } else if (state->keys[i] == EIKU_KEY_STATE_RELEASED) {
      state->keys[i] = EIKU_KEY_STATE_UP;
    }
  }
}