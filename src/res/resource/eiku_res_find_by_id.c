/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_find_by_id.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:45:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:45:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API t_res* eiku_res_find_by_id(t_res_manager* manager, eiku_res_id_t id) {
  if (!manager || id == EIKU_INVALID_RES_ID) {
    return NULL;  // Invalid parameters
  }

  // Search through the resource list
  t_res* current = manager->resources;
  while (current) {
    if (current->id == id) {
      return current;  // Found the resource with matching ID
    }
    current = current->next;
  }

  return NULL;  // Resource not found
}