/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_set_hidden.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:38:13 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:38:37 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

EIKU_API int mouse_set_hidden(t_mouse* mouse, bool hidden) {
  if (!mouse) return -1;
  if (mouse->hidden == hidden) return 0;  // No change needed

#ifdef EIKU_PLATFORM_LINUX
  if (eiku_mouse_set_hidden(mouse, hidden) != 0) {
    return -1;  // Failed to set hidden state
  }
#endif

  return 0;  // Success
}