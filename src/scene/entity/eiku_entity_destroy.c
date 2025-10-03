/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_destroy(t_eiku_entity *entity) {
  size_t i;

  if (!entity) return;
  // Release sprite resource
  if (entity->sprite_res_id != EIKU_INVALID_RES_ID) {
    eiku_res_release_by_id(entity->ctx->res_manager, entity->sprite_res_id);
  }
  // Release animation resources
  if (entity->animation_res_id) {
    for (i = 0; i < entity->animation_count; ++i) {
      if (entity->animation_res_id[i] != EIKU_INVALID_RES_ID) {
        eiku_res_release_by_id(entity->ctx->res_manager,
                               entity->animation_res_id[i]);
      }
    }
    free(entity->animation_res_id);
  }
  // Release data resource
  if (entity->data_res_id != EIKU_INVALID_RES_ID) {
    eiku_res_release_by_id(entity->ctx->res_manager, entity->data_res_id);
  }
  if (entity->name) free(entity->name);
  free(entity);
}
