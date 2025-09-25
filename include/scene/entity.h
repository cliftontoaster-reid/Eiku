/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:26 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/21 18:22:21 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/core.h"
#include "core/platform.h"

// Forward declarations
typedef struct s_eiku_scene eiku_scene_t;
typedef struct s_eiku_entity eiku_entity_t;

// Entity handle - 64-bit stable handle (32-bit index + 32-bit generation)
typedef uint64_t eiku_entity_handle_t;
#define EIKU_ENTITY_NULL ((eiku_entity_handle_t)0)

// Entity types for organization and filtering
typedef enum e_eiku_entity_type {
    EIKU_ENTITY_TYPE_GENERIC = 0,
    EIKU_ENTITY_TYPE_SPRITE,
    EIKU_ENTITY_TYPE_COLLIDER,
    EIKU_ENTITY_TYPE_CAMERA,
    EIKU_ENTITY_TYPE_LIGHT,
    EIKU_ENTITY_TYPE_COUNT
} eiku_entity_type_t;

// Component types (basic set for the initial implementation)
typedef enum e_eiku_component_type {
    EIKU_COMPONENT_TRANSFORM = 0,
    EIKU_COMPONENT_RENDERABLE,
    EIKU_COMPONENT_COLLIDER,
    EIKU_COMPONENT_SCRIPT,
    EIKU_COMPONENT_COUNT
} eiku_component_type_t;

// Entity structure - stores minimal metadata and hierarchy
typedef struct s_eiku_entity {
    uint32_t index;                     // Index in entity array
    uint32_t generation;                // Generation counter for handle validation
    eiku_entity_type_t type;            // Entity type for filtering
    
    // Hierarchy (parent-child relationships)
    eiku_entity_handle_t parent;        // Parent entity handle
    eiku_entity_handle_t first_child;   // First child entity handle
    eiku_entity_handle_t next_sibling;  // Next sibling entity handle
    
    // Component references (bitflags for fast checking)
    uint32_t component_mask;            // Bitmask of attached components
    uint32_t component_ids[EIKU_COMPONENT_COUNT]; // IDs pointing to component data
    
    bool active;                        // Whether entity is active
} eiku_entity_t;

// Entity creation and destruction
EIKU_API eiku_entity_handle_t eiku_entity_create(eiku_scene_t* scene, eiku_entity_type_t type);
EIKU_API void eiku_entity_destroy(eiku_scene_t* scene, eiku_entity_handle_t entity);

// Entity validation
EIKU_API bool eiku_entity_is_valid(eiku_scene_t* scene, eiku_entity_handle_t entity);

// Entity hierarchy management
EIKU_API void eiku_entity_set_parent(eiku_scene_t* scene, eiku_entity_handle_t child, eiku_entity_handle_t parent);
EIKU_API eiku_entity_handle_t eiku_entity_get_parent(eiku_scene_t* scene, eiku_entity_handle_t entity);
EIKU_API eiku_entity_handle_t eiku_entity_first_child(eiku_scene_t* scene, eiku_entity_handle_t entity);
EIKU_API eiku_entity_handle_t eiku_entity_next_sibling(eiku_scene_t* scene, eiku_entity_handle_t entity);

// Entity properties
EIKU_API eiku_entity_type_t eiku_entity_get_type(eiku_scene_t* scene, eiku_entity_handle_t entity);
EIKU_API bool eiku_entity_is_active(eiku_scene_t* scene, eiku_entity_handle_t entity);
EIKU_API void eiku_entity_set_active(eiku_scene_t* scene, eiku_entity_handle_t entity, bool active);

// Component management
EIKU_API bool eiku_entity_has_component(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type);
EIKU_API uint32_t eiku_entity_get_component_id(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type);
EIKU_API void eiku_entity_set_component_id(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type, uint32_t component_id);
EIKU_API void eiku_entity_remove_component(eiku_scene_t* scene, eiku_entity_handle_t entity, eiku_component_type_t component_type);

// Entity iteration helpers
EIKU_API eiku_entity_handle_t eiku_entity_get_first(eiku_scene_t* scene);
EIKU_API eiku_entity_handle_t eiku_entity_get_next(eiku_scene_t* scene, eiku_entity_handle_t entity);
