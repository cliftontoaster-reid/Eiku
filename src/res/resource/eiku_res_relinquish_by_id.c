/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_relinquish_by_id.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:45:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:45:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API void eiku_res_relinquish_by_id(t_res_manager* manager,
                                        eiku_res_id_t id) {
  if (!manager || id == EIKU_INVALID_RES_ID) {
    return;  // Invalid parameters
  }

  // Find the resource by ID
  t_res* res = eiku_res_find_by_id(manager, id);
  if (!res) {
    return;  // Resource not found
  }

  // Use the existing relinquish function
  eiku_res_relinquish(manager, res);
}