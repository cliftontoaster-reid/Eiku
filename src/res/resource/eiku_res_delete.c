/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_delete.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "res/resource.h"

EIKU_INT void eiku_res_delete(t_res_manager* manager, t_res* res) {
  if (!manager || !res) {
    return;  // Invalid parameters
  }

  if (res->type != -1) {
    // Find the resource type to get the delete function
    t_res_type* res_type = find_res_type(manager, res->type);
    if (res_type && res_type->del) {
      res_type->del(res);  // Call the custom delete function
    }
  } else {
    // For type -1, it's a simple free blob, so free data and path directly
    free(res->data);
    if (res->path) free(res->path);
  }

  // We link the prev and the next if both exist
  if (res->prev && res->next) {
    res->prev->next = res->next;
    res->next->prev = res->prev;
  } else if (res->prev) {
    // If only prev exists, we are at the end of the list
    res->prev->next = NULL;
  } else if (res->next) {
    // If only next exists, we are at the start of the list
    res->next->prev = NULL;
    // Update manager's resource head if needed
    if (manager->resources == res) {
      manager->resources = res->next;
    }
  } else {
    // This was the only resource in the list
    if (manager->resources == res) {
      manager->resources = NULL;
    }
  }

  // Finally, free the resource structure itself
  free(res);
}