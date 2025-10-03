/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

float eiku_entity_get_rotation(const t_eiku_entity *entity) {
  if (!entity) return 0.0f;
  return entity->rotation;
}
