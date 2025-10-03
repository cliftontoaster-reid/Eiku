/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_data_res(t_eiku_entity *entity, eiku_res_id_t res_id) {
  if (!entity) return;
  // Release old resource if valid
  if (entity->data_res_id != EIKU_INVALID_RES_ID) {
    eiku_res_release_by_id(entity->ctx->res_manager, entity->data_res_id);
  }
  // Acquire new resource if valid
  if (res_id != EIKU_INVALID_RES_ID) {
    res_id = eiku_res_acquire_by_id(entity->ctx->res_manager, res_id);
  }
  entity->data_res_id = res_id;
}
