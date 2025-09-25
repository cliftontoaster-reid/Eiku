/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_scene_internal.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/scene.h"
#include "scene/component.h"

eiku_entity_t* eiku_scene_get_entity(eiku_scene_t* scene, eiku_entity_handle_t handle)
{
    uint32_t index;
    uint32_t generation;
    
    if (!scene || handle == EIKU_ENTITY_NULL)
        return NULL;
    
    index = (uint32_t)(handle & 0xFFFFFFFF);
    generation = (uint32_t)(handle >> 32);
    
    if (index >= scene->entity_manager.max_entities)
        return NULL;
    
    if (scene->entity_manager.entities[index].generation != generation)
        return NULL;
    
    if (!scene->entity_manager.entities[index].active)
        return NULL;
    
    return &scene->entity_manager.entities[index];
}

eiku_entity_handle_t eiku_scene_allocate_entity(eiku_scene_t* scene)
{
    uint32_t index;
    uint32_t generation;
    eiku_entity_t* entity;
    
    if (!scene)
        return EIKU_ENTITY_NULL;
    
    // Check if we can reuse a free index
    if (scene->entity_manager.free_count > 0)
    {
        scene->entity_manager.free_count--;
        index = scene->entity_manager.free_indices[scene->entity_manager.free_count];
    }
    else
    {
        // Check if we have space for a new entity
        if (scene->entity_manager.entity_count >= scene->entity_manager.max_entities)
            return EIKU_ENTITY_NULL;
        
        index = scene->entity_manager.entity_count;
    }
    
    // Initialize the entity
    entity = &scene->entity_manager.entities[index];
    generation = scene->entity_manager.next_generation++;
    
    entity->index = index;
    entity->generation = generation;
    entity->type = EIKU_ENTITY_TYPE_GENERIC;
    entity->parent = EIKU_ENTITY_NULL;
    entity->first_child = EIKU_ENTITY_NULL;
    entity->next_sibling = EIKU_ENTITY_NULL;
    entity->component_mask = 0;
    
    for (int i = 0; i < EIKU_COMPONENT_COUNT; i++)
        entity->component_ids[i] = EIKU_INVALID_COMPONENT_ID;
    
    entity->active = true;
    
    scene->entity_manager.entity_count++;
    
    return ((uint64_t)generation << 32) | index;
}

void eiku_scene_free_entity(eiku_scene_t* scene, eiku_entity_handle_t handle)
{
    eiku_entity_t* entity;
    uint32_t index;
    
    if (!scene)
        return;
    
    entity = eiku_scene_get_entity(scene, handle);
    if (!entity)
        return;
    
    index = (uint32_t)(handle & 0xFFFFFFFF);
    
    // Mark entity as inactive
    entity->active = false;
    entity->generation++; // Invalidate existing handles
    
    // Add to free list if there's space
    if (scene->entity_manager.free_count < EIKU_SCENE_ENTITY_FREELIST_SIZE)
    {
        scene->entity_manager.free_indices[scene->entity_manager.free_count] = index;
        scene->entity_manager.free_count++;
    }
    
    scene->entity_manager.entity_count--;
}

void eiku_scene_add_root_entity(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    if (!scene || scene->root_entity_count >= scene->max_root_entities)
        return;
    
    scene->root_entities[scene->root_entity_count] = entity;
    scene->root_entity_count++;
}

void eiku_scene_remove_root_entity(eiku_scene_t* scene, eiku_entity_handle_t entity)
{
    uint32_t i;
    
    if (!scene)
        return;
    
    for (i = 0; i < scene->root_entity_count; i++)
    {
        if (scene->root_entities[i] == entity)
        {
            // Move last element to this position
            scene->root_entity_count--;
            scene->root_entities[i] = scene->root_entities[scene->root_entity_count];
            break;
        }
    }
}