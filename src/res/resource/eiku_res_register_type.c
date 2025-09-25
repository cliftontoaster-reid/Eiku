/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_register_type.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/resource.h"

EIKU_API int eiku_res_register_type(t_res_manager* manager, int id,
                                    const char* name, void (*del)(t_res*),
                                    void (*copy)(t_res*, const t_res*)) {
  if (!manager || !name || !del || !copy) {
    return -1;  // Invalid parameters
  }

  // Check if type ID or name already exists
  t_res_type* current = manager->types;
  while (current) {
    if (current->id == id || strcmp(current->name, name) == 0) {
      return -2;  // Type ID or name already registered
    }
    current = current->next;
  }

  // Create and initialize new resource type
  t_res_type* new_type = (t_res_type*)malloc(sizeof(t_res_type));
  if (!new_type) {
    return -3;  // Memory allocation failed
  }
  new_type->id = id;
  new_type->name = name;
  new_type->del = del;
  new_type->copy = copy;

  // Insert the new type at the beginning of the list
  new_type->next = manager->types;
  manager->types = new_type;

  return 0;  // Success
}