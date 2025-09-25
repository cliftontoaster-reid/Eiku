/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_entity_iteration.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/entity.h"
#include "scene/scene.h"

eiku_entity_handle_t eiku_entity_get_first(eiku_scene_t* scene)
{
    if (!scene || scene->root_entity_count == 0)
        return EIKU_ENTITY_NULL;
    
    return scene->root_entities[0];
}

eiku_entity_handle_t eiku_entity_get_next(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    uint32_t i;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return EIKU_ENTITY_NULL;
    
    // Find the entity in the root entities list and return the next one
    for (i = 0; i < scene->root_entity_count - 1; i++)
    {
        if (scene->root_entities[i] == entity)
            return scene->root_entities[i + 1];
    }
    
    return EIKU_ENTITY_NULL;
}