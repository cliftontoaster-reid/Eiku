/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_keyboard_clear_text.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 01:49:41 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 01:49:49 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API void eiku_keyboard_clear_text(t_eiku_keyboard_state *state) {
  if (!state) return;
  state->text_length = 0;
  state->text_buffer[0] = '\0';
}