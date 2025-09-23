/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_keyboard_set_repeats.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 01:00:16 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 01:57:29 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/keyboard.h"

EIKU_API void eiku_keyboard_set_repeats(t_eiku_keyboard_state *state,
                                        bool enable) {
  if (!state) return;
  state->enable_key_repeats = enable;
}