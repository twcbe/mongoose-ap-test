#include "mgos.h"
#include "mgos_wifi.h"
#include "mgos_wifi_captive_portal.h"
#define LAMBDA(c_) ({ c_ _; })
int counter = 0;
int disconnects = 0;

static void http_cb(struct mg_connection *c, int ev, void *ev_data, void *ud)
{

  switch (ev)
  {
  case MG_EV_CONNECT:
    break;
  case MG_EV_HTTP_CHUNK:
  {
    LOG(LL_INFO, ("chunk"));
    c->flags |= MG_F_DELETE_CHUNK;
    break;
  }
  case MG_EV_HTTP_REPLY:

    LOG(LL_INFO, ("reply"));
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
  static bool s_tick_tock = false;
  LOG(LL_INFO,
      ("%s uptime: %.2lf, RAM: %lu, %lu free", (s_tick_tock ? "Tick" : "Tock"),
       mgos_uptime(), (unsigned long)mgos_get_heap_size(),
       (unsigned long)mgos_get_free_heap_size()));
  s_tick_tock = !s_tick_tock;
  (void)arg;
}

static void net_cb(int ev, void *evd, void *arg)
{

  switch (ev)
  {
  case MGOS_NET_EV_DISCONNECTED:
    LOG(LL_INFO, ("%s", "Net disconnected"));
    break;
  case MGOS_NET_EV_CONNECTING:
    LOG(LL_INFO, ("%s", "Net connecting..."));
    break;
  case MGOS_NET_EV_CONNECTED:
    LOG(LL_INFO, ("%s", "Net connected"));
    break;
  case MGOS_NET_EV_IP_ACQUIRED:
    LOG(LL_INFO, ("%s", "Net got IP address"));
    // mg_connect_http(mgos_get_mgr(), http_cb, NULL, "http://www.google.com", NULL, NULL);
    break;
  }

  (void)evd;

  (void)arg;
}

static void wifi_cb(int ev, void *evd, void *arg)
{
  switch (ev)
  {
  case MGOS_WIFI_EV_STA_DISCONNECTED:
    LOG(LL_INFO, ("WiFi STA disconnected %p", arg));
    // mgos_config_set_wifi_ap_enable(&mgos_sys_config, 1);
    // mgos_usleep(500);
    // mgos_system_restart();
    disconnects++;
    if (disconnects >= 5)
    {
      LOG(LL_INFO, ("WiFi STA disconnected %d times, Starting portal", disconnects));
      mgos_sys_config_set_wifi_sta_enable(false);
      mgos_sys_config_set_wifi_ap_enable(true);
      mgos_wifi_captive_portal_start();
      mgos_system_restart();
      disconnects = 0;
    }
    break;
  case MGOS_WIFI_EV_STA_CONNECTING:
    LOG(LL_INFO, ("WiFi STA connecting %p", arg));
    break;
  case MGOS_WIFI_EV_STA_CONNECTED:
    LOG(LL_INFO, ("WiFi STA connected %p", arg));
    break;
  case MGOS_WIFI_EV_STA_IP_ACQUIRED:
    LOG(LL_INFO, ("WiFi STA IP acquired %p", arg));
    break;
  case MGOS_WIFI_EV_AP_STA_CONNECTED:
  {
    struct mgos_wifi_ap_sta_connected_arg *aa =
        (struct mgos_wifi_ap_sta_connected_arg *)evd;
    LOG(LL_INFO, ("WiFi AP STA connected MAC %02x:%02x:%02x:%02x:%02x:%02x",
                  aa->mac[0], aa->mac[1], aa->mac[2], aa->mac[3], aa->mac[4],
                  aa->mac[5]));

    LOG(LL_INFO, ("WiFi AP STA connected MAC %d %d %d %d %d %d",
                  aa->mac[0], aa->mac[1], aa->mac[2], aa->mac[3], aa->mac[4],
                  aa->mac[5]));

    const char *ssid = mgos_config_get_wifi_ap_ssid(&mgos_sys_config);
    LOG(LL_INFO, ("Wifi ssid is %s", ssid));
    break;
  }
  case MGOS_WIFI_EV_AP_STA_DISCONNECTED:
  {
    struct mgos_wifi_ap_sta_disconnected_arg *aa =
        (struct mgos_wifi_ap_sta_disconnected_arg *)evd;
    LOG(LL_INFO,
        ("WiFi AP STA disconnected MAC %02x:%02x:%02x:%02x:%02x:%02x",
         aa->mac[0], aa->mac[1], aa->mac[2], aa->mac[3], aa->mac[4],
         aa->mac[5]));
    break;
  }
  }
  (void)arg;
}

static int func(int(f)(int))
{
  return f(100);
}

static int id(int x) { return x; }

typedef int (*int_identity)(int param);

static int_identity get_id_function()
{
  return id;
}

enum mgos_app_init_result
mgos_app_init(void)
{
  mgos_set_timer(35000, MGOS_TIMER_REPEAT, timer_cb, NULL);
  mgos_event_add_group_handler(MGOS_WIFI_EV_BASE, wifi_cb, NULL);
  mgos_event_add_group_handler(MGOS_EVENT_GRP_NET, net_cb, NULL);

  // uint8_t mac[6];
  // esp_wifi_get_mac(ESP_IF_WIFI_AP, mac);
  // LOG(LL_ERROR, ("Local MAC Address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]));

  // // char mac_str[6];
  // uint8_t mac_uin8t[6];
  // device_get_mac_address(mac_uin8t);
  // int i;
  // for (i = 0; i < 6; ++i)
  // {
  //   LOG(LL_INFO, ("%d", mac_uin8t[i]));
  // } // sprintf(mac_str, "%02x%02x%02x%02x%02x%02x\n", mac_uin8t[0] & 0xFF, mac_uin8t[1] & 0xFF, mac_uin8t[2] & 0xFF, mac_uin8t[3] & 0xFF, mac_uin8t[4] & 0xFF, mac_uin8t[5] & 0xFF);

  int_identity int_id = LAMBDA(int _(int v) {
    return v;
  });
  int result1 = func(get_id_function());
  int result = func(LAMBDA(int _(int v) {
    return v;
  }));
  LOG(LL_INFO, ("%d", result));
  return MGOS_APP_INIT_SUCCESS;
}
