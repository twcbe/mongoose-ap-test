#include "mgos.h"
#include "mgos_wifi.h"

const struct mgos_config_wifi_sta config;

enum mgos_app_init_result mgos_app_init(void)
{
  mgos_wifi_setup_sta(&config);
  return MGOS_APP_INIT_SUCCESS;
}