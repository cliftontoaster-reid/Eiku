/* ************************************************************************** */
#include "scene/entity.h"
#include "scene/utility.h"

void eiku_set_player_direction(t_eiku_entity *player, t_vec2f dir) {
  if (!player || !player->ctx) return;
  eiku_res_id_t id = eiku_entity_get_data_res(player);
  if (id == EIKU_INVALID_RES_ID) return;

  t_res *res = eiku_res_find_by_id(player->ctx->res_manager, id);
  if (!res || !res->data) return;

  t_eiku_player_data *pdata = (t_eiku_player_data *)res->data;
  pdata->dir = dir;
}
