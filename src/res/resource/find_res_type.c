/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_res_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:17:24 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:33:09 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_INT t_res_type* find_res_type(t_res_manager* manager, int type_id) {
  t_res_type* current = manager->types;
  while (current) {
    if (current->id == type_id) {
      return current;
    }
    current = current->next;
  }
  return NULL;  // Type not found
}
