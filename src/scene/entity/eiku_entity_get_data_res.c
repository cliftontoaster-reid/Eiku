/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

eiku_res_id_t eiku_entity_get_data_res(const t_eiku_entity *entity) {
  if (!entity) return EIKU_INVALID_RES_ID;
  return entity->data_res_id;
}
