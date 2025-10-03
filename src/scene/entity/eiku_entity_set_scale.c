/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_scale(t_eiku_entity *entity, t_vec2f scale) {
  if (!entity) return;
  entity->scale = scale;
}
