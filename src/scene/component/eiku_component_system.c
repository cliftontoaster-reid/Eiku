/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_component_system.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/component.h"
#include "scene/scene.h"
#include <stdlib.h>
#include <string.h>

// Simple global component storage for demo purposes
// In a full implementation, this would be part of the scene structure
static transform_component_t* transform_components = NULL;
static renderable_component_t* renderable_components = NULL;
static collider_component_t* collider_components = NULL;
static script_component_t* script_components = NULL;

static uint32_t transform_count = 0;
static uint32_t renderable_count = 0;
static uint32_t collider_count = 0;
static uint32_t script_count = 0;

static uint32_t next_component_id = 1; // Start from 1, 0 is invalid

// Initialize component storage on first use
static void ensure_component_storage(void)
{
    if (!transform_components)
    {
        transform_components = calloc(EIKU_MAX_COMPONENTS_PER_TYPE, sizeof(transform_component_t));
        renderable_components = calloc(EIKU_MAX_COMPONENTS_PER_TYPE, sizeof(renderable_component_t));
        collider_components = calloc(EIKU_MAX_COMPONENTS_PER_TYPE, sizeof(collider_component_t));
        script_components = calloc(EIKU_MAX_COMPONENTS_PER_TYPE, sizeof(script_component_t));
    }
}

uint32_t eiku_component_attach(eiku_scene_t* scene, eiku_entity_handle_t entity, 
                               eiku_component_type_t component_type, const void* component_data)
{
    uint32_t component_id;
    
    if (!scene || entity == EIKU_ENTITY_NULL || !component_data || component_type >= EIKU_COMPONENT_COUNT)
        return EIKU_INVALID_COMPONENT_ID;
    
    if (!eiku_entity_is_valid(scene, entity))
        return EIKU_INVALID_COMPONENT_ID;
    
    ensure_component_storage();
    component_id = next_component_id++;
    
    switch (component_type)
    {
        case EIKU_COMPONENT_TRANSFORM:
            if (transform_count >= EIKU_MAX_COMPONENTS_PER_TYPE)
                return EIKU_INVALID_COMPONENT_ID;
            memcpy(&transform_components[transform_count], component_data, sizeof(transform_component_t));
            transform_count++;
            break;
            
        case EIKU_COMPONENT_RENDERABLE:
            if (renderable_count >= EIKU_MAX_COMPONENTS_PER_TYPE)
                return EIKU_INVALID_COMPONENT_ID;
            memcpy(&renderable_components[renderable_count], component_data, sizeof(renderable_component_t));
            renderable_count++;
            break;
            
        case EIKU_COMPONENT_COLLIDER:
            if (collider_count >= EIKU_MAX_COMPONENTS_PER_TYPE)
                return EIKU_INVALID_COMPONENT_ID;
            memcpy(&collider_components[collider_count], component_data, sizeof(collider_component_t));
            collider_count++;
            break;
            
        case EIKU_COMPONENT_SCRIPT:
            if (script_count >= EIKU_MAX_COMPONENTS_PER_TYPE)
                return EIKU_INVALID_COMPONENT_ID;
            memcpy(&script_components[script_count], component_data, sizeof(script_component_t));
            script_count++;
            break;
            
        default:
            return EIKU_INVALID_COMPONENT_ID;
    }
    
    // Attach component to entity
    eiku_entity_set_component_id(scene, entity, component_type, component_id);
    
    return component_id;
}

void eiku_component_detach(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type)
{
    if (!scene || entity == EIKU_ENTITY_NULL || component_type >= EIKU_COMPONENT_COUNT)
        return;
    
    eiku_entity_remove_component(scene, entity, component_type);
}

void* eiku_component_get_data(eiku_scene_t* scene, eiku_component_type_t component_type, uint32_t component_id)
{
    (void)scene; // Unused in this simple implementation
    
    if (component_id == EIKU_INVALID_COMPONENT_ID || component_type >= EIKU_COMPONENT_COUNT)
        return NULL;
    
    ensure_component_storage();
    
    // Simple linear search for demo purposes
    // In a real implementation, this would use proper indexing
    switch (component_type)
    {
        case EIKU_COMPONENT_TRANSFORM:
            return transform_count > 0 ? &transform_components[0] : NULL;
        case EIKU_COMPONENT_RENDERABLE:
            return renderable_count > 0 ? &renderable_components[0] : NULL;
        case EIKU_COMPONENT_COLLIDER:
            return collider_count > 0 ? &collider_components[0] : NULL;
        case EIKU_COMPONENT_SCRIPT:
            return script_count > 0 ? &script_components[0] : NULL;
        default:
            return NULL;
    }
}

bool eiku_component_set_data(eiku_scene_t* scene, eiku_component_type_t component_type, 
                            uint32_t component_id, const void* component_data)
{
    void* data;
    
    if (!component_data)
        return false;
    
    data = eiku_component_get_data(scene, component_type, component_id);
    if (!data)
        return false;
    
    switch (component_type)
    {
        case EIKU_COMPONENT_TRANSFORM:
            memcpy(data, component_data, sizeof(transform_component_t));
            break;
        case EIKU_COMPONENT_RENDERABLE:
            memcpy(data, component_data, sizeof(renderable_component_t));
            break;
        case EIKU_COMPONENT_COLLIDER:
            memcpy(data, component_data, sizeof(collider_component_t));
            break;
        case EIKU_COMPONENT_SCRIPT:
            memcpy(data, component_data, sizeof(script_component_t));
            break;
        default:
            return false;
    }
    
    return true;
}

// Type-specific helper functions
transform_component_t* eiku_component_get_transform(eiku_scene_t* scene, uint32_t component_id)
{
    return (transform_component_t*)eiku_component_get_data(scene, EIKU_COMPONENT_TRANSFORM, component_id);
}

renderable_component_t* eiku_component_get_renderable(eiku_scene_t* scene, uint32_t component_id)
{
    return (renderable_component_t*)eiku_component_get_data(scene, EIKU_COMPONENT_RENDERABLE, component_id);
}

collider_component_t* eiku_component_get_collider(eiku_scene_t* scene, uint32_t component_id)
{
    return (collider_component_t*)eiku_component_get_data(scene, EIKU_COMPONENT_COLLIDER, component_id);
}

script_component_t* eiku_component_get_script(eiku_scene_t* scene, uint32_t component_id)
{
    return (script_component_t*)eiku_component_get_data(scene, EIKU_COMPONENT_SCRIPT, component_id);
}