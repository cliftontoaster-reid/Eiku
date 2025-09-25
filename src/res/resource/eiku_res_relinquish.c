/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_relinquish.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "res/resource.h"

EIKU_API void eiku_res_relinquish(t_res_manager* manager, t_res* res) {
  if (!manager || !res) {
    return;  // Invalid parameters
  }

  // Only relinquish if the resource is marked as writable
  if (res->writeable) {
    res->writeable = false;  // Mark as no longer writable

    // If this is a temporary copy created by eiku_res_acquire_writable,
    // automatically release it to prevent memory leaks
    if (res->temporary) {
      eiku_res_release(manager, res);
    }
  }
}