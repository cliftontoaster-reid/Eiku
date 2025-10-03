/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_rotation(t_eiku_entity *entity, float rotation) {
  if (!entity) return;
  entity->rotation = rotation;
}
