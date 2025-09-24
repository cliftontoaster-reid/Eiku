/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:41:28 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:42:09 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "input/mouse.h"

EIKU_API int mouse_init(t_mouse* mouse, t_eiku_context* ctx,
                        t_eiku_window* window) {
  if (!mouse || !ctx || !window) {
    return -1;  // Invalid parameters
  }
  memset(mouse, 0, sizeof(t_mouse));

  mouse->ctx = ctx;
  mouse->window = window;

  return 0;
}