/* ************************************************************************** */
#include "scene/entity.h"
#include "scene/utility.h"

bool eiku_is_collectible_collected(const t_eiku_entity *collectible) {
  if (!collectible || !collectible->ctx) return false;
  eiku_res_id_t id = eiku_entity_get_data_res(collectible);
  if (id == EIKU_INVALID_RES_ID) return false;

  t_res *res = eiku_res_find_by_id(collectible->ctx->res_manager, id);
  if (!res || !res->data) return false;

  t_eiku_collectible_data *cdata = (t_eiku_collectible_data *)res->data;
  return cdata->collected;
}
