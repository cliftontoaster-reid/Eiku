/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_active(t_eiku_entity *entity, bool active) {
  if (!entity) return;
  entity->active = active;
}
