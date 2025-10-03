/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

t_vec2f eiku_entity_get_position(const t_eiku_entity *entity) {
  if (!entity) return (t_vec2f){0.0f, 0.0f};
  return entity->position;
}
