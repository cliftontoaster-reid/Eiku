/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"
#include "scene/utility.h"

t_eiku_entity *eiku_create_collectible(t_eiku_context *ctx, const char *name,
                                       t_vec2f position) {
  t_eiku_entity *ent;

  if (!ctx) return NULL;
  ent = eiku_entity_create(ctx, name ? name : "collectible");
  if (!ent) return NULL;

  eiku_entity_set_position(ent, position);
  eiku_entity_set_scale(ent, vec2f_one());
  eiku_entity_set_rotation(ent, 0.0f);
  eiku_entity_set_active(ent, true);

  if (ctx->res_manager) {
    t_res *res = eiku_res_create(ctx->res_manager, EIKU_DATA_TYPE_COLLECTIBLE,
                                 sizeof(t_eiku_collectible_data), false);
    if (res) {
      t_eiku_collectible_data *cdata = (t_eiku_collectible_data *)res->data;
      cdata->collected = false;
      eiku_entity_set_data_res(ent, res->id);
      eiku_res_release(ctx->res_manager, res);
    }
  }

  return ent;
}
