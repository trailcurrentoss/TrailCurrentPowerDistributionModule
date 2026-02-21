#include <debug.h>
#include "globals.h"
#include "canHelper.h"
#include <OtaUpdate.h>
#include "lightSequences.h"
#include "wifiConfig.h"

// Global credential buffers - writable at runtime
char runtimeSsid[33] = {0};
char runtimePassword[64] = {0};

// Create OTA update handler (3-minute timeout, 180000 ms)
OtaUpdate otaUpdate(180000, runtimeSsid, runtimePassword);

void setup()
{
  Serial.begin(115200);

#if DEBUG == 1
  // Wait for serial monitor connection (up to 3 seconds)
  // Ensures startup messages are visible when monitor connects after boot
  unsigned long serialWait = millis();
  while (!Serial.available() && (millis() - serialWait < 3000)) {
    delay(10);
  }
#endif

  debugln("\n=== TrailCurrent Power Control Module ===");
  debugln("CAN-Controlled 8-Channel PWM Lighting");

  // Initialize WiFi config storage and load stored credentials
  wifiConfig::init();
  wifiConfig::setRuntimeCredentialPtrs(runtimeSsid, runtimePassword);

  // Load credentials from NVS (provisioned via CAN bus message 0x01)
  if (wifiConfig::loadCredentials(runtimeSsid, runtimePassword)) {
    debugln("[WiFi] Loaded credentials from NVS");
  } else {
    debugln("[WiFi] No credentials in NVS - OTA disabled until provisioned via CAN");
  }

  // Initialize output pins
  pinMode(OUTPUT01_PIN, OUTPUT);
  pinMode(OUTPUT02_PIN, OUTPUT);
  pinMode(OUTPUT03_PIN, OUTPUT);
  pinMode(OUTPUT04_PIN, OUTPUT);
  pinMode(OUTPUT05_PIN, OUTPUT);
  pinMode(OUTPUT06_PIN, OUTPUT);
  pinMode(OUTPUT07_PIN, OUTPUT);
  pinMode(OUTPUT08_PIN, OUTPUT);

  // Run the startup light show
  debugln("[LIGHTS] Starting 30-second light show...");
  //lightSequences::startupLightShow();
  debugln("[LIGHTS] Light show complete!");

  // Initialize OTA (connects to WiFi)
  debugf("[OTA] Device hostname: %s\n", otaUpdate.getHostName().c_str());
  debugln("[OTA] Ready to receive OTA trigger (CAN ID 0x0)");

  // Initialize CAN
  canHelper::setupCan();

  debugln("=== Setup Complete ===\n");
}

void loop()
{
  canHelper::canLoop();
  canHelper::send_status_message();
}
