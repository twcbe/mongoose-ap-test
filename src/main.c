#include "mgos.h"
#include "mgos_wifi.h"

int counter = 0;

static void http_cb(struct mg_connection *c, int ev, void *ev_data, void *ud)
{

  switch (ev)
  {
  case MG_EV_CONNECT:
    break;
  case MG_EV_HTTP_CHUNK:
  {

    LOG(LL_INFO, ("Finished fetching "));
    c->flags |= MG_F_DELETE_CHUNK;
    break;
  }
  case MG_EV_HTTP_REPLY:

    LOG(LL_INFO, ("Finished fetching"));
    c->flags |= MG_F_CLOSE_IMMEDIATELY;
    break;
  case MG_EV_CLOSE:
    LOG(LL_INFO, ("status"));
    break;
  }
}

static void timer_cb(void *arg)
{
  LOG(LL_INFO, ("counter: %d", counter++));
  (void)arg;
}

enum mgos_app_init_result mgos_app_init(void)
{

  mgos_set_timer(35000, MGOS_TIMER_REPEAT, timer_cb, NULL);
  mg_connect_http(mgos_get_mgr(), http_cb, NULL, "http://www.google.com", NULL, NULL);
  // mgos_set_wifi_ap_enable(&mgos_sys_config, false);
  return MGOS_APP_INIT_SUCCESS;
}
