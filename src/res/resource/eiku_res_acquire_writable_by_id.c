/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_acquire_writable_by_id.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:45:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:45:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API eiku_res_id_t eiku_res_acquire_writable_by_id(t_res_manager* manager,
                                                       eiku_res_id_t id) {
  if (!manager || id == EIKU_INVALID_RES_ID) {
    return EIKU_INVALID_RES_ID;  // Invalid parameters
  }

  // Find the resource by ID
  t_res* res = eiku_res_find_by_id(manager, id);
  if (!res) {
    return EIKU_INVALID_RES_ID;  // Resource not found
  }

  // Use the existing acquire_writable function
  t_res* acquired = eiku_res_acquire_writable(manager, res);
  if (!acquired) {
    return EIKU_INVALID_RES_ID;  // Acquire failed
  }

  return acquired
      ->id;  // Return the resource ID (may be a new copy with different ID)
}