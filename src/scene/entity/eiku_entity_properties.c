/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_entity_properties.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/entity.h"
#include "scene/scene.h"

bool eiku_entity_is_valid(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    return eiku_scene_get_entity(scene, entity) != NULL;
}

eiku_entity_type_t eiku_entity_get_type(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return EIKU_ENTITY_TYPE_GENERIC;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return EIKU_ENTITY_TYPE_GENERIC;
    
    return entity_data->type;
}

bool eiku_entity_is_active(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return false;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return false;
    
    return entity_data->active;
}

void eiku_entity_set_active(eiku_scene_t* scene, eiku_entity_handle_t entity, bool active)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return;
    
    entity_data->active = active;
}