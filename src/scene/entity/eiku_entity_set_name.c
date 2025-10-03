/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

void eiku_entity_set_name(t_eiku_entity *entity, const char *name) {
  if (!entity) return;
  if (entity->name) {
    free(entity->name);
    entity->name = NULL;
  }
  if (name) entity->name = strdup(name);
}
