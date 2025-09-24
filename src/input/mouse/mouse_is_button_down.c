/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_is_button_down.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:31:59 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:32:27 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

EIKU_API bool mouse_is_button_down(t_mouse* mouse, int button) {
  if (button < 1 || button > 8) {
    return false;  // Invalid button
  }
  return mouse->buttons[button - 1];
}
