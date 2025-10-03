/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

eiku_res_id_t eiku_entity_get_animation_res(const t_eiku_entity *entity,
                                            size_t index) {
  if (!entity || index >= entity->animation_count) return EIKU_INVALID_RES_ID;
  return entity->animation_res_id[index];
}
