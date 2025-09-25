/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_manager_free.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "res/resource.h"

EIKU_API void eiku_res_manager_free(t_res_manager* manager) {
  if (!manager) {
    return;  // Nothing to free
  }

  // Free all resources
  t_res* current = manager->resources;
  while (current) {
    t_res* next = current->next;
    eiku_res_delete(manager, current);
    current = next;
  }

  // Free all resource types
  t_res_type* type_current = manager->types;
  while (type_current) {
    t_res_type* type_next = type_current->next;
    free(type_current);
    type_current = type_next;
  }

  // Finally, free the manager itself
  free(manager);
}
