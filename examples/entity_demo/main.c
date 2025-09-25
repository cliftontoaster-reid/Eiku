// Example usage of the Entity System API.
// This demonstrates creating entities, setting up a parent-child hierarchy,
// attaching components, and iterating over children.
// Assumes headers like include/scene/entity.h and include/scene/scene.h exist.
// filepath: examples/entity_demo/main.c

#include <stdio.h>
#include <stdlib.h>
#include "scene/scene.h"      // For eiku_scene_t
#include "scene/entity.h"     // For entity API
#include "scene/component.h"  // For component types and attach functions

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    // Initialize a scene (assumes eiku_scene_create exists)
    eiku_scene_t *scene = eiku_scene_create();
    if (!scene) {
        fprintf(stderr, "Failed to create scene\n");
        return EXIT_FAILURE;
    }

    printf("Entity System Demo\n");
    printf("==================\n\n");

    // Create a root entity (e.g., a sprite object)
    eiku_entity_handle_t sprite = eiku_entity_create(scene, EIKU_ENTITY_TYPE_SPRITE);
    if (sprite == EIKU_ENTITY_NULL) {
        fprintf(stderr, "Failed to create sprite entity\n");
        eiku_scene_destroy(scene);
        return EXIT_FAILURE;
    }
    
    printf("Created sprite entity with handle: %llu\n", (unsigned long long)sprite);

    // Attach a transform component to the sprite
    transform_component_t sprite_transform = {10.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    uint32_t transform_id = eiku_component_attach(scene, sprite, EIKU_COMPONENT_TRANSFORM, &sprite_transform);
    if (transform_id == EIKU_INVALID_COMPONENT_ID) {
        fprintf(stderr, "Failed to attach transform\n");
        eiku_entity_destroy(scene, sprite);
        eiku_scene_destroy(scene);
        return EXIT_FAILURE;
    }
    
    printf("Attached transform component (ID: %u) to sprite at position (%.1f, %.1f, %.1f)\n", 
           transform_id, sprite_transform.x, sprite_transform.y, sprite_transform.z);

    // Attach a renderable component
    renderable_component_t sprite_render = {42, 1, true}; // texture ID = 42, material ID = 1, visible = true
    uint32_t render_id = eiku_component_attach(scene, sprite, EIKU_COMPONENT_RENDERABLE, &sprite_render);
    
    if (render_id != EIKU_INVALID_COMPONENT_ID) {
        printf("Attached renderable component (ID: %u) with texture ID %u\n", render_id, sprite_render.texture_id);
    }

    // Create a child entity (e.g., a collider attached to the sprite)
    eiku_entity_handle_t collider = eiku_entity_create(scene, EIKU_ENTITY_TYPE_COLLIDER);
    if (collider == EIKU_ENTITY_NULL) {
        fprintf(stderr, "Failed to create collider entity\n");
        eiku_entity_destroy(scene, sprite);
        eiku_scene_destroy(scene);
        return EXIT_FAILURE;
    }
    
    printf("Created collider entity with handle: %llu\n", (unsigned long long)collider);

    // Attach a collider component
    collider_component_t sprite_collider = {5.0f, false}; // radius = 5.0, is_trigger = false
    uint32_t collider_id = eiku_component_attach(scene, collider, EIKU_COMPONENT_COLLIDER, &sprite_collider);
    
    if (collider_id != EIKU_INVALID_COMPONENT_ID) {
        printf("Attached collider component (ID: %u) with radius %.1f\n", collider_id, sprite_collider.radius);
    }

    // Set the collider as a child of the sprite
    eiku_entity_set_parent(scene, collider, sprite);
    printf("Set collider as child of sprite\n");

    // Verify hierarchy
    eiku_entity_handle_t parent = eiku_entity_get_parent(scene, collider);
    if (parent == sprite) {
        printf("Hierarchy verified: collider's parent is the sprite\n");
    }

    // Demonstrate iteration: print all children of the sprite
    printf("\nIterating over children of sprite entity:\n");
    for (eiku_entity_handle_t child = eiku_entity_first_child(scene, sprite);
         child != EIKU_ENTITY_NULL;
         child = eiku_entity_next_sibling(scene, child)) {
        printf("  Child entity handle: %llu\n", (unsigned long long)child);
        
        // Check what components this child has
        if (eiku_entity_has_component(scene, child, EIKU_COMPONENT_COLLIDER)) {
            printf("    Has collider component\n");
        }
        if (eiku_entity_has_component(scene, child, EIKU_COMPONENT_TRANSFORM)) {
            printf("    Has transform component\n");
        }
        if (eiku_entity_has_component(scene, child, EIKU_COMPONENT_RENDERABLE)) {
            printf("    Has renderable component\n");
        }
    }

    // Demonstrate scene statistics
    printf("\nScene Statistics:\n");
    printf("  Entity count: %u\n", eiku_scene_get_entity_count(scene));
    printf("  Root entity count: %u\n", eiku_scene_get_root_entity_count(scene));
    printf("  Scene name: %s\n", eiku_scene_get_name(scene));

    // Test entity validation
    printf("\nEntity Validation:\n");
    printf("  Sprite is valid: %s\n", eiku_entity_is_valid(scene, sprite) ? "true" : "false");
    printf("  Collider is valid: %s\n", eiku_entity_is_valid(scene, collider) ? "true" : "false");

    // Test invalid entity
    eiku_entity_handle_t invalid_entity = 0xDEADBEEF;
    printf("  Invalid entity is valid: %s\n", eiku_entity_is_valid(scene, invalid_entity) ? "true" : "false");

    // Clean up
    printf("\nCleaning up...\n");
    eiku_entity_destroy(scene, sprite); // This should also destroy the collider child
    eiku_scene_destroy(scene);

    printf("Entity demo completed successfully.\n");
    return EXIT_SUCCESS;
}