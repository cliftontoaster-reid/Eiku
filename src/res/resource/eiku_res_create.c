/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "res/resource.h"

EIKU_API t_res* eiku_res_create(t_res_manager* manager, int type, size_t size,
                                bool safe) {
  if (!manager || size == 0) {
    return NULL;  // Invalid parameters
  }

  // Get the resource type to ensure it's registered
  if (!find_res_type(manager, type)) {
    return NULL;  // Resource type not registered
  }

  // Allocate and initialize the new resource
  t_res* new_res = (t_res*)malloc(sizeof(t_res));
  if (!new_res) {
    return NULL;  // Memory allocation failed
  }
  new_res->data = malloc(size);
  if (!new_res->data) {
    free(new_res);
    return NULL;  // Memory allocation for data failed
  }
  new_res->id = manager->next_id++;  // Assign unique auto-incremented ID
  new_res->size = size;
  new_res->type = type;
  new_res->path = NULL;  // No path assigned yet
  new_res->modified = false;
  new_res->safe = safe;
  new_res->ref_count = 1;      // Initial reference count
  new_res->writeable = false;  // Not aquired as writeable
  new_res->temporary = false;  // Not a temporary copy by default
  new_res->prev = NULL;
  new_res->next = NULL;

  // Insert the new resource at the beginning of the manager's resource list
  if (manager->resources) {
    new_res->next = manager->resources;
    manager->resources->prev = new_res;
  }
  manager->resources = new_res;

  return new_res;  // Return the newly created resource
}