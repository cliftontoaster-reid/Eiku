/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_animations(t_eiku_entity *entity, eiku_res_id_t *res_ids,
                                size_t count) {
  size_t i;
  eiku_res_id_t *copy;

  if (!entity) return;
  // Release old animations
  if (entity->animation_res_id) {
    for (i = 0; i < entity->animation_count; ++i) {
      if (entity->animation_res_id[i] != EIKU_INVALID_RES_ID) {
        eiku_res_release_by_id(entity->ctx->res_manager,
                               entity->animation_res_id[i]);
      }
    }
    free(entity->animation_res_id);
  }
  if (!res_ids || count == 0) {
    entity->animation_res_id = NULL;
    entity->animation_count = 0;
    entity->current_animation = 0;
    entity->animation_time = 0.0f;
    return;
  }
  copy = (eiku_res_id_t *)malloc(sizeof(eiku_res_id_t) * count);
  if (!copy) {
    entity->animation_res_id = NULL;
    entity->animation_count = 0;
    return;
  }
  for (i = 0; i < count; ++i) {
    copy[i] = res_ids[i];
    // Acquire each resource
    if (copy[i] != EIKU_INVALID_RES_ID) {
      copy[i] = eiku_res_acquire_by_id(entity->ctx->res_manager, copy[i]);
    }
  }
  entity->animation_res_id = copy;
  entity->animation_count = count;
  entity->current_animation = 0;
  entity->animation_time = 0.0f;
}
