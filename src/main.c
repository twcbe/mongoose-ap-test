#include "mgos.h"
#include "mgos_wifi.h"

const struct mgos_config_wifi_ap config;

enum mgos_app_init_result mgos_app_init(void)
{
  // config.enable = 1;
  // config.ssid = "Incubator__??????";
  // config.pass = "12345678";
  // config.hidden = 0;
  // config.channel = 6;
  // config.max_connections = 2;
  // config.ip = "192.168.4.1";         // Static IP Address
  // config.netmask = "255.255.255.0";  // Static Netmas
  // config.gw = "192.168.4.1";         // Static Default Gatway
  // config.dhcp_start = "192.168.4.2"; // DHCP Start Addres
  // config.dhcp_end = "192.168.4.100"; // DHCP End Addres
  // config.trigger_on_gpio = -1;       // Trigger AP on low GPI

  mgos_wifi_setup_ap(&config);
  return MGOS_APP_INIT_SUCCESS;
}