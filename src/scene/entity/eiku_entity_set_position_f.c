/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_position_f(t_eiku_entity *entity, float x, float y) {
  if (!entity) return;
  entity->position.x = x;
  entity->position.y = y;
}
