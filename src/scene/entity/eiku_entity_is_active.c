/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

bool eiku_entity_is_active(const t_eiku_entity *entity) {
  if (!entity) return false;
  return entity->active;
}
