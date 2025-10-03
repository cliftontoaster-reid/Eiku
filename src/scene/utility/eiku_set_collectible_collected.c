/* ************************************************************************** */
#include "scene/entity.h"
#include "scene/utility.h"

void eiku_set_collectible_collected(t_eiku_entity *collectible,
                                    bool collected) {
  if (!collectible || !collectible->ctx) return;
  eiku_res_id_t id = eiku_entity_get_data_res(collectible);
  if (id == EIKU_INVALID_RES_ID) return;

  t_res *res = eiku_res_find_by_id(collectible->ctx->res_manager, id);
  if (!res || !res->data) return;

  t_eiku_collectible_data *cdata = (t_eiku_collectible_data *)res->data;
  cdata->collected = collected;
}
