#ifndef _SCREEN_WIFI_SETTINGS_H
#define _SCREEN_WIFI_SETTINGS_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "TouchToggle.h"
#include "DisplayToastBox.h"

class ScreenWiFiSettings: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchToggle enable;

    // Constructor
    ScreenWiFiSettings();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setWiFiName(const char *wifiName);   // Set the WiFi name / SSID
    void setIPAddress(const char *ipAddress); // Set the WiFi IP address

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    // Screen elements
    DisplayToastBox _wifiName;
    DisplayToastBox _wifiPassword;
    DisplayToastBox _wifiIPAddress;

};

#endif