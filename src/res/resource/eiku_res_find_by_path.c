/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_find_by_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "res/resource.h"

EIKU_API t_res* eiku_res_find_by_path(t_res_manager* manager,
                                      const char* path) {
  if (!manager || !path) {
    return NULL;  // Invalid parameters
  }

  t_res* current = manager->resources;
  while (current) {
    if (current->path && strcmp(current->path, path) == 0) {
      return current;  // Found the resource
    }
    current = current->next;
  }
  return NULL;  // Resource not found
}