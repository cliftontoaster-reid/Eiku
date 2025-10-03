/* ************************************************************************** */
#include <stdlib.h>
#include <string.h>

#include "scene/entity.h"
#include "scene/utility.h"

t_eiku_entity *eiku_create_player(t_eiku_context *ctx, const char *name,
                                  t_vec2f position) {
  t_eiku_entity *player;

  if (!ctx) return NULL;
  player = eiku_entity_create(ctx, name ? name : "player");
  if (!player) return NULL;

  /* Setup transform */
  eiku_entity_set_position(player, position);
  eiku_entity_set_scale(player, vec2f_one());
  eiku_entity_set_rotation(player, 0.0f);
  eiku_entity_set_active(player, true);

  /* Allocate and attach player data resource (if resource system used) */
  /* We create a small data resource to store player-specific data and mark
     its type as EIKU_DATA_TYPE_PLAYER. The resource manager will own it. */
  t_res *res = NULL;
  if (ctx->res_manager) {
    res = eiku_res_create(ctx->res_manager, EIKU_DATA_TYPE_PLAYER,
                          sizeof(t_eiku_player_data), false);
    if (res) {
      /* initialize */
      t_eiku_player_data *pdata = (t_eiku_player_data *)res->data;
      pdata->dir = vec2f_zero();
      /* register and acquire by id */
      eiku_entity_set_data_res(player, res->id);
      /* release local reference because set_data_res acquired it */
      eiku_res_release(ctx->res_manager, res);
    }
  }

  return player;
}
