/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_entity_hierarchy.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/entity.h"
#include "scene/scene.h"

void eiku_entity_set_parent(eiku_scene_t* scene, eiku_entity_handle_t child, eiku_entity_handle_t parent)
{
    eiku_entity_t* child_entity;
    eiku_entity_t* parent_entity;
    eiku_entity_t* old_parent_entity;
    eiku_entity_t* sibling_entity;
    
    if (!scene || child == EIKU_ENTITY_NULL)
        return;
    
    child_entity = eiku_scene_get_entity(scene, child);
    if (!child_entity)
        return;
    
    // Remove from old parent's child list
    if (child_entity->parent != EIKU_ENTITY_NULL)
    {
        old_parent_entity = eiku_scene_get_entity(scene, child_entity->parent);
        if (old_parent_entity && old_parent_entity->first_child == child)
        {
            // Child is first child, update parent's first_child
            old_parent_entity->first_child = child_entity->next_sibling;
        }
        else if (old_parent_entity)
        {
            // Find and remove child from sibling list
            eiku_entity_handle_t prev_sibling = old_parent_entity->first_child;
            sibling_entity = eiku_scene_get_entity(scene, prev_sibling);
            
            while (sibling_entity && sibling_entity->next_sibling != child)
            {
                prev_sibling = sibling_entity->next_sibling;
                sibling_entity = eiku_scene_get_entity(scene, prev_sibling);
            }
            
            if (sibling_entity)
                sibling_entity->next_sibling = child_entity->next_sibling;
        }
    }
    else
    {
        // Remove from root entities
        eiku_scene_remove_root_entity(scene, child);
    }
    
    // Set new parent
    child_entity->parent = parent;
    child_entity->next_sibling = EIKU_ENTITY_NULL;
    
    if (parent == EIKU_ENTITY_NULL)
    {
        // Add to root entities
        eiku_scene_add_root_entity(scene, child);
    }
    else
    {
        parent_entity = eiku_scene_get_entity(scene, parent);
        if (parent_entity)
        {
            // Add as first child
            child_entity->next_sibling = parent_entity->first_child;
            parent_entity->first_child = child;
        }
    }
}

eiku_entity_handle_t eiku_entity_get_parent(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return EIKU_ENTITY_NULL;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return EIKU_ENTITY_NULL;
    
    return entity_data->parent;
}

eiku_entity_handle_t eiku_entity_first_child(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return EIKU_ENTITY_NULL;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return EIKU_ENTITY_NULL;
    
    return entity_data->first_child;
}

eiku_entity_handle_t eiku_entity_next_sibling(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    eiku_entity_t* entity_data;
    
    if (!scene || entity == EIKU_ENTITY_NULL)
        return EIKU_ENTITY_NULL;
    
    entity_data = eiku_scene_get_entity(scene, entity);
    if (!entity_data)
        return EIKU_ENTITY_NULL;
    
    return entity_data->next_sibling;
}