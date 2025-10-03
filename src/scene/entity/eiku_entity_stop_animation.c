/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_stop_animation(t_eiku_entity *entity) {
  if (!entity) return;
  entity->current_animation = 0;
  entity->animation_time = 0.0f;
}
