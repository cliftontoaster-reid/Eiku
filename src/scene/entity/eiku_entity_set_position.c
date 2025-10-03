/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_position(t_eiku_entity *entity, t_vec2f pos) {
  if (!entity) return;
  entity->position = pos;
}
