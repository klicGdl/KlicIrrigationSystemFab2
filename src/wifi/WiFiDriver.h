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
#pragma once

#include "WiFiManager.h"

namespace WiFiDriver {

/**
 * @brief WiFi Configurations.
 */
namespace Config {
/**
 * Specifies the ESP32 host wifi ssid.
 */
extern const char* WIFI_SSID;
/**
 * Specifies the ESP32 host wifi pass.
 */
extern const char* WIFI_PASS;
}  // namespace Config


extern WiFiManager _wm;

/**
 * @brief Initialize WIFI interface using the specified configuration.
 *
 * @return bool   WiFi status code.
 */
bool WiFiInitialize(void);
}  // namespace WiFiDriver
