/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:16 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/21 18:22:23 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/core.h"
#include "core/platform.h"
#include "scene/entity.h"

// Forward declarations
typedef struct s_eiku_scene eiku_scene_t;

// Scene configuration constants
#define EIKU_SCENE_MAX_ENTITIES 65536
#define EIKU_SCENE_ENTITY_FREELIST_SIZE 1024

// Entity storage and management structure
typedef struct s_eiku_entity_manager {
    eiku_entity_t* entities;        // Dense array of entities
    uint32_t* free_indices;         // Freelist for reusing entity indices
    uint32_t entity_count;          // Current number of active entities
    uint32_t max_entities;          // Maximum number of entities
    uint32_t free_count;            // Number of free indices available
    uint32_t next_generation;       // Generation counter for new entities
} eiku_entity_manager_t;

// Scene structure - manages entities and their relationships
typedef struct s_eiku_scene {
    eiku_entity_manager_t entity_manager;
    
    // Root entities (entities without parents) for efficient iteration
    eiku_entity_handle_t* root_entities;
    uint32_t root_entity_count;
    uint32_t max_root_entities;
    
    // Scene metadata
    char* name;
    bool active;
} eiku_scene_t;

// Scene lifecycle management
EIKU_API eiku_scene_t* eiku_scene_create(void);
EIKU_API eiku_scene_t* eiku_scene_create_with_name(const char* name);
EIKU_API void eiku_scene_destroy(eiku_scene_t* scene);

// Scene properties
EIKU_API const char* eiku_scene_get_name(eiku_scene_t* scene);
EIKU_API void eiku_scene_set_name(eiku_scene_t* scene, const char* name);
EIKU_API bool eiku_scene_is_active(eiku_scene_t* scene);
EIKU_API void eiku_scene_set_active(eiku_scene_t* scene, bool active);

// Scene statistics
EIKU_API uint32_t eiku_scene_get_entity_count(eiku_scene_t* scene);
EIKU_API uint32_t eiku_scene_get_root_entity_count(eiku_scene_t* scene);

// Internal entity manager access (used by entity functions)
EIKU_INT eiku_entity_t* eiku_scene_get_entity(eiku_scene_t* scene, eiku_entity_handle_t handle);
EIKU_INT eiku_entity_handle_t eiku_scene_allocate_entity(eiku_scene_t* scene);
EIKU_INT void eiku_scene_free_entity(eiku_scene_t* scene, eiku_entity_handle_t handle);
EIKU_INT void eiku_scene_add_root_entity(eiku_scene_t* scene, eiku_entity_handle_t entity);
EIKU_INT void eiku_scene_remove_root_entity(eiku_scene_t* scene, eiku_entity_handle_t entity);
