/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_entity_system.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:00:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 16:00:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include "scene/scene.h"
#include "scene/entity.h"
#include "scene/component.h"

TestSuite(entity_system, .description = "Entity System tests");

Test(entity_system, test_scene_creation)
{
    eiku_scene_t* scene = eiku_scene_create();
    
    cr_assert_not_null(scene, "Scene should be created successfully");
    cr_assert_str_eq(eiku_scene_get_name(scene), "Untitled Scene", "Default scene name should be 'Untitled Scene'");
    cr_assert(eiku_scene_is_active(scene), "Scene should be active by default");
    cr_assert_eq(eiku_scene_get_entity_count(scene), 0, "New scene should have no entities");
    cr_assert_eq(eiku_scene_get_root_entity_count(scene), 0, "New scene should have no root entities");
    
    eiku_scene_destroy(scene);
}

Test(entity_system, test_entity_creation_and_destruction)
{
    eiku_scene_t* scene = eiku_scene_create();
    cr_assert_not_null(scene);
    
    // Test entity creation
    eiku_entity_handle_t entity = eiku_entity_create(scene, EIKU_ENTITY_TYPE_SPRITE);
    cr_assert_neq(entity, EIKU_ENTITY_NULL, "Entity should be created successfully");
    cr_assert(eiku_entity_is_valid(scene, entity), "Created entity should be valid");
    cr_assert_eq(eiku_scene_get_entity_count(scene), 1, "Scene should have 1 entity");
    cr_assert_eq(eiku_scene_get_root_entity_count(scene), 1, "Scene should have 1 root entity");
    
    // Test entity properties
    cr_assert_eq(eiku_entity_get_type(scene, entity), EIKU_ENTITY_TYPE_SPRITE, "Entity should have correct type");
    cr_assert(eiku_entity_is_active(scene, entity), "Entity should be active by default");
    
    // Test entity destruction
    eiku_entity_destroy(scene, entity);
    cr_assert(!eiku_entity_is_valid(scene, entity), "Destroyed entity should be invalid");
    cr_assert_eq(eiku_scene_get_entity_count(scene), 0, "Scene should have 0 entities after destruction");
    
    eiku_scene_destroy(scene);
}

Test(entity_system, test_entity_hierarchy)
{
    eiku_scene_t* scene = eiku_scene_create();
    cr_assert_not_null(scene);
    
    // Create parent and child entities
    eiku_entity_handle_t parent = eiku_entity_create(scene, EIKU_ENTITY_TYPE_SPRITE);
    eiku_entity_handle_t child = eiku_entity_create(scene, EIKU_ENTITY_TYPE_COLLIDER);
    
    cr_assert_neq(parent, EIKU_ENTITY_NULL);
    cr_assert_neq(child, EIKU_ENTITY_NULL);
    cr_assert_eq(eiku_scene_get_root_entity_count(scene), 2, "Should have 2 root entities initially");
    
    // Set up parent-child relationship
    eiku_entity_set_parent(scene, child, parent);
    
    // Test hierarchy
    cr_assert_eq(eiku_entity_get_parent(scene, child), parent, "Child should have correct parent");
    cr_assert_eq(eiku_entity_first_child(scene, parent), child, "Parent should have correct first child");
    cr_assert_eq(eiku_entity_next_sibling(scene, child), EIKU_ENTITY_NULL, "Child should have no sibling");
    cr_assert_eq(eiku_scene_get_root_entity_count(scene), 1, "Should have 1 root entity after hierarchy setup");
    
    // Test removing from hierarchy
    eiku_entity_set_parent(scene, child, EIKU_ENTITY_NULL);
    cr_assert_eq(eiku_entity_get_parent(scene, child), EIKU_ENTITY_NULL, "Child should have no parent");
    cr_assert_eq(eiku_entity_first_child(scene, parent), EIKU_ENTITY_NULL, "Parent should have no children");
    cr_assert_eq(eiku_scene_get_root_entity_count(scene), 2, "Should have 2 root entities again");
    
    eiku_entity_destroy(scene, parent);
    eiku_entity_destroy(scene, child);
    eiku_scene_destroy(scene);
}

Test(entity_system, test_component_system)
{
    eiku_scene_t* scene = eiku_scene_create();
    cr_assert_not_null(scene);
    
    eiku_entity_handle_t entity = eiku_entity_create(scene, EIKU_ENTITY_TYPE_SPRITE);
    cr_assert_neq(entity, EIKU_ENTITY_NULL);
    
    // Test component attachment
    transform_component_t transform = {1.0f, 2.0f, 3.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    uint32_t transform_id = eiku_component_attach(scene, entity, EIKU_COMPONENT_TRANSFORM, &transform);
    
    cr_assert_neq(transform_id, EIKU_INVALID_COMPONENT_ID, "Component should be attached successfully");
    cr_assert(eiku_entity_has_component(scene, entity, EIKU_COMPONENT_TRANSFORM), "Entity should have transform component");
    cr_assert_eq(eiku_entity_get_component_id(scene, entity, EIKU_COMPONENT_TRANSFORM), transform_id, "Entity should have correct component ID");
    
    // Test component data access
    transform_component_t* retrieved = eiku_component_get_transform(scene, transform_id);
    cr_assert_not_null(retrieved, "Component data should be retrievable");
    
    // Test component removal
    eiku_entity_remove_component(scene, entity, EIKU_COMPONENT_TRANSFORM);
    cr_assert(!eiku_entity_has_component(scene, entity, EIKU_COMPONENT_TRANSFORM), "Entity should not have transform component after removal");
    
    eiku_entity_destroy(scene, entity);
    eiku_scene_destroy(scene);
}