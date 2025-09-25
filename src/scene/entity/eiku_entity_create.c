/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_entity_create.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/entity.h"
#include "scene/scene.h"

eiku_entity_handle_t eiku_entity_create(eiku_scene_t* scene, eiku_entity_type_t type)
{
    eiku_entity_handle_t handle;
    eiku_entity_t* entity;
    
    if (!scene || type >= EIKU_ENTITY_TYPE_COUNT)
        return EIKU_ENTITY_NULL;
    
    handle = eiku_scene_allocate_entity(scene);
    if (handle == EIKU_ENTITY_NULL)
        return EIKU_ENTITY_NULL;
    
    entity = eiku_scene_get_entity(scene, handle);
    if (!entity)
        return EIKU_ENTITY_NULL;
    
    entity->type = type;
    
    // Add to root entities (no parent)
    eiku_scene_add_root_entity(scene, handle);
    
    return handle;
}

void eiku_entity_destroy(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    eiku_entity_t* entity_data;
    eiku_entity_handle_t child;
    eiku_entity_handle_t next_child;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return;
    
    // Destroy all children recursively
    child = entity_data->first_child;
    while (child != EIKU_ENTITY_NULL)
    {
        next_child = eiku_entity_next_sibling(scene, child);
        eiku_entity_destroy(scene, child);
        child = next_child;
    }
    
    // Remove from parent's child list if it has a parent
    if (entity_data->parent != EIKU_ENTITY_NULL)
    {
        eiku_entity_set_parent(scene, entity, EIKU_ENTITY_NULL);
    }
    else
    {
        // Remove from root entities
        eiku_scene_remove_root_entity(scene, entity);
    }
    
    // Free the entity
    eiku_scene_free_entity(scene, entity);
}