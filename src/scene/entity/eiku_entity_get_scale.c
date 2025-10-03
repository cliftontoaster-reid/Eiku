/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

t_vec2f eiku_entity_get_scale(const t_eiku_entity *entity) {
  if (!entity) return (t_vec2f){1.0f, 1.0f};
  return entity->scale;
}
