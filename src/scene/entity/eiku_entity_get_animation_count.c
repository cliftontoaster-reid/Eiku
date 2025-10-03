/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

size_t eiku_entity_get_animation_count(const t_eiku_entity *entity) {
  if (!entity) return 0;
  return entity->animation_count;
}
