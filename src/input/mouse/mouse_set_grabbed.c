/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_set_grabbed.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:39:53 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:40:23 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

EIKU_API int mouse_set_grabbed(t_mouse* mouse, bool grabbed) {
  if (!mouse || !mouse->ctx || !mouse->window) return -1;

  mouse->grabbed = grabbed;

  return 0;  // Success
}