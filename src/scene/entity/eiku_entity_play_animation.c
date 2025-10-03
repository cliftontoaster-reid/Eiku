/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_play_animation(t_eiku_entity *entity, size_t index) {
  if (!entity || index >= entity->animation_count) return;
  entity->current_animation = index;
  entity->animation_time = 0.0f;
}
