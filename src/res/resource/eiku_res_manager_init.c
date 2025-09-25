/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_manager_init.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "res/resource.h"

EIKU_API t_res_manager* eiku_res_manager_init(void) {
  t_res_manager* manager = (t_res_manager*)malloc(sizeof(t_res_manager));
  if (!manager) {
    return NULL;  // Memory allocation failed
  }
  manager->resources = NULL;
  manager->types = NULL;
  manager->next_id = 1;  // Start IDs from 1 (0 is reserved for invalid ID)
  return manager;
}
