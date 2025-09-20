/******************************************************************************
 * Project: Klic Smart Irrigation System
 *
 * Description:
 * Library intended to initialize the WiFi drivers for an Arduino-based ESP32
 * CPU
 *
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited
 *    without explicit permission from the author
 ******************************************************************************/
#include "WiFiDriver.h"

#include "utils/logger.h"

void _configureWiFiManager() {
  WiFiDriver::_wm.setConnectTimeout(120);
  WiFiDriver::_wm.setConfigPortalTimeout(180);
}

namespace WiFiDriver {

// Namespace extern definitions
namespace Config {
const char* WIFI_SSID = "KLIC-ISFAB2";
const char* WIFI_PASS = "ROOT";
}  // namespace Config

WiFiManager _wm;

// Exposed methods

bool WiFiInitialize(void) {
  WiFi.mode(WIFI_STA);
  _configureWiFiManager();

  // will try to connect when it has saved credentials
  // if it fails will start the access point
  bool connected = WiFiDriver::_wm.autoConnect(WiFiDriver::Config::WIFI_SSID);
<<<<<<< HEAD
    if (!connected) {
=======

  if (!connected) {
>>>>>>> origin/main
    logger << LOG_ERROR << "WiFi AutoConnect failed to: "
           << WiFiDriver::_wm.getWiFiSSID(false)
           << " with pass: " << WiFiDriver::_wm.getWiFiPass(false) << EndLine;
    return false;
  }

  logger << LOG_INFO << "WiFi AutoConnect done! IP: " << WiFi.localIP()
         << EndLine;

  return true;
}

}  // namespace WiFiDriver

