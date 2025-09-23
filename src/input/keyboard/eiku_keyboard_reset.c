/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_keyboard_reset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 01:50:26 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 01:50:47 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API void eiku_keyboard_reset(t_eiku_keyboard_state *state) {
  if (!state) return;

  for (size_t i = 0; i < EIKU_KEY_COUNT; ++i) {
    state->keys[i] = EIKU_KEY_STATE_UP;
  }
  state->modifiers = EIKU_MOD_NONE;
  eiku_keyboard_clear_text(state);
}