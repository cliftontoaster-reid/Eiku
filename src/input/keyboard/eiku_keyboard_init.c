/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_keyboard_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 01:00:16 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 01:57:29 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API int eiku_keyboard_init(t_eiku_context *ctx,
                                t_eiku_keyboard_state *state) {
  if (!ctx || !state) return 0;

  state->ctx = ctx;
  for (size_t i = 0; i < EIKU_KEY_COUNT; ++i) {
    state->keys[i] = EIKU_KEY_STATE_UP;
  }
  state->modifiers = EIKU_MOD_NONE;
  state->text_length = 0;
  state->text_buffer[0] = '\0';
  state->enable_key_repeats = false;

  return 1; /* Success */
}