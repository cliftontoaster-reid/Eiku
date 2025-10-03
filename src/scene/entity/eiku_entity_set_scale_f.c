/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_scale_f(t_eiku_entity *entity, float x, float y) {
  if (!entity) return;
  entity->scale.x = x;
  entity->scale.y = y;
}
