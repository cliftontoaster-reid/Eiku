/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_scene_create.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:50:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:50:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/scene.h"
#include <stdlib.h>
#include <string.h>

eiku_scene_t* eiku_scene_create(void)
{
    return eiku_scene_create_with_name("Untitled Scene");
}

eiku_scene_t* eiku_scene_create_with_name(const char* name)
{
    eiku_scene_t* scene;
    
    if (!name)
        return NULL;
    
    scene = calloc(1, sizeof(eiku_scene_t));
    if (!scene)
        return NULL;
    
    // Initialize entity manager
    scene->entity_manager.entities = calloc(EIKU_SCENE_MAX_ENTITIES, sizeof(eiku_entity_t));
    scene->entity_manager.free_indices = calloc(EIKU_SCENE_ENTITY_FREELIST_SIZE, sizeof(uint32_t));
    scene->entity_manager.max_entities = EIKU_SCENE_MAX_ENTITIES;
    scene->entity_manager.entity_count = 0;
    scene->entity_manager.free_count = 0;
    scene->entity_manager.next_generation = 1;
    
    if (!scene->entity_manager.entities || !scene->entity_manager.free_indices)
    {
        free(scene->entity_manager.entities);
        free(scene->entity_manager.free_indices);
        free(scene);
        return NULL;
    }
    
    // Initialize root entity tracking
    scene->root_entities = calloc(EIKU_SCENE_MAX_ENTITIES / 4, sizeof(eiku_entity_handle_t));
    scene->max_root_entities = EIKU_SCENE_MAX_ENTITIES / 4;
    scene->root_entity_count = 0;
    
    if (!scene->root_entities)
    {
        free(scene->entity_manager.entities);
        free(scene->entity_manager.free_indices);
        free(scene);
        return NULL;
    }
    
    // Set scene name
    scene->name = strdup(name);
    if (!scene->name)
    {
        free(scene->root_entities);
        free(scene->entity_manager.entities);
        free(scene->entity_manager.free_indices);
        free(scene);
        return NULL;
    }
    
    scene->active = true;
    
    return scene;
}