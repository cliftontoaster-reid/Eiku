/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_acquire_writable.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API t_res* eiku_res_acquire_writable(t_res_manager* manager, t_res* res) {
  if (!manager || !res) {
    return NULL;  // Invalid parameters
  }

  // If the resource is marked as safe or is writable (already exclusive),
  // create a copy
  if (res->safe || res->writeable) {
    // Find the resource type to get the copy function
    t_res_type* res_type = find_res_type(manager, res->type);
    if (!res_type || !res_type->copy) {
      return NULL;  // Cannot copy without a copy function
    }

    // Create a new resource for the copy
    t_res* new_res = eiku_res_create(manager, res->type, res->size, res->safe);
    if (!new_res) {
      return NULL;  // Failed to create new resource
    }

    // Use the type-specific copy function
    res_type->copy(new_res, res);
    new_res->writeable = true;  // Mark the new resource as writable
    new_res->temporary = true;  // Mark as temporary copy for auto-cleanup

    return new_res;  // Return the new writable resource
  } else {
    // If the resource is not safe and not writable, we can make it writable
    res->writeable = true;
    return res;  // Return the original resource now marked as writable
  }
}