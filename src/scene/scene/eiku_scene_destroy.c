/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_scene_destroy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/scene.h"
#include <stdlib.h>

void eiku_scene_destroy(eiku_scene_t* scene)
{
    if (!scene)
        return;
    
    // Clean up scene name
    free(scene->name);
    
    // Clean up entity manager
    free(scene->entity_manager.entities);
    free(scene->entity_manager.free_indices);
    
    // Clean up root entity tracking
    free(scene->root_entities);
    
    // Free scene structure
    free(scene);
}