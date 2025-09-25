/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_try_acquire_writable.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API t_res* eiku_res_try_acquire_writable(t_res_manager* manager,
                                              t_res* res) {
  if (!manager || !res) {
    return NULL;  // Invalid parameters
  }

  // If the resource is marked as safe or is writable (already exclusive),
  // return NULL since we cannot acquire writable access
  if (res->safe || res->writeable) {
    return NULL;  // Cannot acquire writable access
  } else {
    // If the resource is not safe and not writable, we can make it writable
    res->writeable = true;
    return res;  // Return the original resource now marked as writable
  }
}
