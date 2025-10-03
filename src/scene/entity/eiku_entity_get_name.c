/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

const char *eiku_entity_get_name(const t_eiku_entity *entity) {
  if (!entity) return NULL;
  return entity->name;
}
