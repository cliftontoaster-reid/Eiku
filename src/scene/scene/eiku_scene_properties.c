/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_scene_properties.c                           :+:      :+:    :+:   */
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

const char* eiku_scene_get_name(eiku_scene_t* scene)
{
    if (!scene)
        return NULL;
    return scene->name;
}

void eiku_scene_set_name(eiku_scene_t* scene, const char* name)
{
    if (!scene || !name)
        return;
    
    free(scene->name);
    scene->name = strdup(name);
}

bool eiku_scene_is_active(eiku_scene_t* scene)
{
    if (!scene)
        return false;
    return scene->active;
}

void eiku_scene_set_active(eiku_scene_t* scene, bool active)
{
    if (!scene)
        return;
    scene->active = active;
}

uint32_t eiku_scene_get_entity_count(eiku_scene_t* scene)
{
    if (!scene)
        return 0;
    return scene->entity_manager.entity_count;
}

uint32_t eiku_scene_get_root_entity_count(eiku_scene_t* scene)
{
    if (!scene)
        return 0;
    return scene->root_entity_count;
}