/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_entity_components.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/component.h"

bool eiku_entity_has_component(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL || component_type >= EIKU_COMPONENT_COUNT)
        return false;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return false;
    
    return (entity_data->component_mask & (1 << component_type)) != 0;
}

uint32_t eiku_entity_get_component_id(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL || component_type >= EIKU_COMPONENT_COUNT)
        return EIKU_INVALID_COMPONENT_ID;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return EIKU_INVALID_COMPONENT_ID;
    
    if (!eiku_entity_has_component(scene, entity, component_type))
        return EIKU_INVALID_COMPONENT_ID;
    
    return entity_data->component_ids[component_type];
}

void eiku_entity_set_component_id(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type, uint32_t component_id)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL || component_type >= EIKU_COMPONENT_COUNT)
        return;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return;
    
    entity_data->component_ids[component_type] = component_id;
    
    if (component_id != EIKU_INVALID_COMPONENT_ID)
    {
        // Set the component flag
        entity_data->component_mask |= (1 << component_type);
    }
    else
    {
        // Clear the component flag
        entity_data->component_mask &= ~(1 << component_type);
    }
}

void eiku_entity_remove_component(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type)
{
    eiku_entity_set_component_id(scene, entity, component_type, EIKU_INVALID_COMPONENT_ID);
}