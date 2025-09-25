/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   component.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:25 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/21 18:22:22 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/core.h"
#include "core/platform.h"
#include "scene/entity.h"

// Component storage system (minimal implementation for entity system demo)
// This provides a basic interface for attaching component data to entities
// Full component system implementation would be more sophisticated

// Basic component data structures for demonstration
typedef struct s_transform_component {
    float x, y, z;      // Position
    float sx, sy, sz;   // Scale
    float rx, ry, rz;   // Rotation (Euler angles for now)
} transform_component_t;

typedef struct s_renderable_component {
    uint32_t texture_id;        // Texture resource ID
    uint32_t material_id;       // Material resource ID
    bool visible;               // Visibility flag
} renderable_component_t;

typedef struct s_collider_component {
    float radius;               // Simple sphere collider for now
    bool is_trigger;            // Whether collider is a trigger
} collider_component_t;

typedef struct s_script_component {
    uint32_t script_id;         // Script resource ID
    void* user_data;            // User data pointer
} script_component_t;

// Component attach/detach functions
// These create component data and return IDs that entities can reference
EIKU_API uint32_t eiku_component_attach(eiku_scene_t* scene, eiku_entity_handle_t entity, 
                                        eiku_component_type_t component_type, const void* component_data);
EIKU_API void eiku_component_detach(eiku_scene_t* scene, eiku_entity_handle_t entity, 
                                   eiku_component_type_t component_type);

// Component data access functions
EIKU_API void* eiku_component_get_data(eiku_scene_t* scene, eiku_component_type_t component_type, uint32_t component_id);
EIKU_API bool eiku_component_set_data(eiku_scene_t* scene, eiku_component_type_t component_type, 
                                     uint32_t component_id, const void* component_data);

// Type-specific helper functions for convenience
EIKU_API transform_component_t* eiku_component_get_transform(eiku_scene_t* scene, uint32_t component_id);
EIKU_API renderable_component_t* eiku_component_get_renderable(eiku_scene_t* scene, uint32_t component_id);
EIKU_API collider_component_t* eiku_component_get_collider(eiku_scene_t* scene, uint32_t component_id);
EIKU_API script_component_t* eiku_component_get_script(eiku_scene_t* scene, uint32_t component_id);

// Component system constants
#define EIKU_INVALID_COMPONENT_ID ((uint32_t)0)
#define EIKU_MAX_COMPONENTS_PER_TYPE 4096
