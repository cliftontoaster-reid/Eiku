/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_release.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API void eiku_res_release(t_res_manager* manager, t_res* res) {
  if (!manager || !res) {
    return;  // Invalid parameters
  }

  // Decrement the reference count
  if (res->ref_count > 0) {
    res->ref_count--;
  }

  // If the reference count reaches zero, delete the resource
  if (res->ref_count == 0) {
    eiku_res_delete(manager, res);
  }
}
