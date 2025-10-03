/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"

/* Allocate and initialize a new entity with sensible defaults */
t_eiku_entity *eiku_entity_create(t_eiku_context *ctx, const char *name) {
  t_eiku_entity *e;

  e = (t_eiku_entity *)malloc(sizeof(*e));
  if (!e) return NULL;
  memset(e, 0, sizeof(*e));
  e->ctx = ctx;
  e->id = 0;
  e->name = name ? strdup(name) : NULL;
  e->position.x = 0.0f;
  e->position.y = 0.0f;
  e->scale.x = 1.0f;
  e->scale.y = 1.0f;
  e->rotation = 0.0f;
  e->active = true;
  e->sprite_res_id = EIKU_INVALID_RES_ID;
  e->animation_res_id = NULL;
  e->animation_count = 0;
  e->current_animation = 0;
  e->animation_time = 0.0f;
  e->data_res_id = EIKU_INVALID_RES_ID;
  e->init = NULL;
  e->update = NULL;
  e->destroy = NULL;
  return e;
}
