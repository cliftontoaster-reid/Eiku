/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

/* Advance the animation time accumulator. Real frame switching requires
         additional metadata (frame durations) stored in the animation resource.
         This function only updates the time counter; the rendering/user code is
         expected to query current animation index and resource and decide which
         frame to display. */
void eiku_entity_update_animation(t_eiku_entity *entity, float delta_time) {
  if (!entity) return;
  entity->animation_time += delta_time;
}
