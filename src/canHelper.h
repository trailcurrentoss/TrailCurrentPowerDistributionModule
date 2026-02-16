#pragma once
#include "globals.h"
#include <TwaiTaskBased.h>
#include "lightSequences.h"
#include <OtaUpdate.h>
#include "wifiConfig.h"

// Forward declare otaUpdate (defined in main.cpp)
extern OtaUpdate otaUpdate;

#define CAN_RX 13
#define CAN_TX 15
#define CAN_SEND_MESSAGE_ID 0x1B
#define STATUS_TX_INTERVAL_MS 33

int aryLightValues[8] = {0, 0, 0, 0, 0, 0, 0, 0};

namespace canHelper
{
    /**
     * Handle OTA trigger from CAN message ID 0x0
     * Format: 3 bytes [MAC byte 3, MAC byte 4, MAC byte 5]
     * Example: 0x8A, 0x3B, 0x4C triggers device esp32-8A3B4C
     */
    void handleOtaTrigger(const uint8_t *data) {
        char updateForHostName[14];
        String currentHostName = otaUpdate.getHostName();

        // Format: esp32-XXXXXX where X is MAC address in hex
        sprintf(updateForHostName, "esp32-%02X%02X%02X",
                data[0], data[1], data[2]);

        debugf("[OTA] Target hostname: %s\n", updateForHostName);
        debugf("[OTA] Current hostname: %s\n", currentHostName.c_str());

        // Check if this OTA trigger is for this device
        if (currentHostName.equals(updateForHostName)) {
            debugln("[OTA] Hostname matched - entering OTA mode");
            otaUpdate.waitForOta();  // Blocking call, waits for OTA update or timeout
            debugln("[OTA] OTA mode exited - resuming normal operation");
        } else {
            debugln("[OTA] Hostname mismatch - ignoring OTA trigger");
        }
    }

    static void handle_rx_message(const twai_message_t &message)
    {
        // Process received message
        if (message.extd)
        {
            debugln("Message is in Extended Format");
        }
        else
        {
            debugln("Message is in Standard Format");
        }
        if (!(message.rtr))
        {
            debugln(message.identifier);
            debugln(message.data[0]);

            // OTA trigger message (ID 0x0)
            if (message.identifier == 0x0 && message.data_length_code >= 3) {
                debugln("[OTA] OTA trigger received");
                handleOtaTrigger(message.data);
            }
            // WiFi configuration message (ID 0x01)
            else if (message.identifier == 0x01 && message.data_length_code >= 1) {
                debugln("[WiFi Config] Received WiFi config message");
                wifiConfig::handleCanMessage(message.data, message.data_length_code);
            }
            else if (message.identifier == 24) // Message ID of 24 are on/off messages
            {
                // This message contains only one byte and the value indiciates which of the 8 toggle requests was made.
                if (message.data[0] == 0)
                {
                    if (aryLightValues[0] > 0)
                    {
                        aryLightValues[0] = 0;
                    }
                    else
                    {
                        aryLightValues[0] = 255;
                    }
                    analogWrite(OUTPUT01_PIN, aryLightValues[0]);
                }
                else if (message.data[0] == 1)
                {
                    if (aryLightValues[1] > 0)
                    {
                        aryLightValues[1] = 0;
                    }
                    else
                    {
                        aryLightValues[1] = 255;
                    }
                    analogWrite(OUTPUT02_PIN, aryLightValues[1]);
                }
                else if (message.data[0] == 2)
                {
                    if (aryLightValues[2] > 0)
                    {
                        aryLightValues[2] = 0;
                    }
                    else
                    {
                        aryLightValues[2] = 255;
                    }
                    analogWrite(OUTPUT03_PIN, aryLightValues[2]);
                }
                else if (message.data[0] == 3)
                {
                    if (aryLightValues[3] > 0)
                    {
                        aryLightValues[3] = 0;
                    }
                    else
                    {
                        aryLightValues[3] = 255;
                    }
                    analogWrite(OUTPUT04_PIN, aryLightValues[3]);
                }
                else if (message.data[0] == 4)
                {
                    if (aryLightValues[4] > 0)
                    {
                        aryLightValues[4] = 0;
                    }
                    else
                    {
                        aryLightValues[4] = 255;
                    }
                    analogWrite(OUTPUT05_PIN, aryLightValues[4]);
                }
                else if (message.data[0] == 5)
                {
                    if (aryLightValues[5] > 0)
                    {
                        aryLightValues[5] = 0;
                    }
                    else
                    {
                        aryLightValues[5] = 255;
                    }
                    analogWrite(OUTPUT06_PIN, aryLightValues[5]);
                }
                else if (message.data[0] == 6)
                {
                    if (aryLightValues[6] > 0)
                    {
                        aryLightValues[6] = 0;
                    }
                    else
                    {
                        aryLightValues[6] = 255;
                    }
                    analogWrite(OUTPUT07_PIN, aryLightValues[6]);
                }
                else if (message.data[0] == 7)
                {
                    if (aryLightValues[7] > 0)
                    {
                        aryLightValues[7] = 0;
                    }
                    else
                    {
                        aryLightValues[7] = 255;
                    }
                    analogWrite(OUTPUT08_PIN, aryLightValues[7]);
                }
                else if (message.data[0] == 8)
                {
                    debugln("Got HERE");
                    debugln(message.data[1]);
                    if (message.data[1] == 0)
                    {
                        aryLightValues[0] = 0;
                        aryLightValues[1] = 0;
                        aryLightValues[2] = 0;
                        aryLightValues[3] = 0;
                        aryLightValues[4] = 0;
                        aryLightValues[5] = 0;
                        aryLightValues[6] = 0;
                        aryLightValues[7] = 0;
                    }
                    else
                    {
                        aryLightValues[0] = 255;
                        aryLightValues[1] = 255;
                        aryLightValues[2] = 255;
                        aryLightValues[3] = 255;
                        aryLightValues[4] = 255;
                        aryLightValues[5] = 255;
                        aryLightValues[6] = 255;
                        aryLightValues[7] = 255;
                    }
                    analogWrite(OUTPUT01_PIN, aryLightValues[0]);
                    analogWrite(OUTPUT02_PIN, aryLightValues[1]);
                    analogWrite(OUTPUT03_PIN, aryLightValues[2]);
                    analogWrite(OUTPUT04_PIN, aryLightValues[3]);
                    analogWrite(OUTPUT05_PIN, aryLightValues[4]);
                    analogWrite(OUTPUT06_PIN, aryLightValues[5]);
                    analogWrite(OUTPUT07_PIN, aryLightValues[6]);
                    analogWrite(OUTPUT08_PIN, aryLightValues[7]);
                }
                else if (message.data[0] == 9)
                {
                    debugln("Got HERE");
                    debugln(message.data[1]);
                    if (message.data[1] == 1)
                    {
                        aryLightValues[0] = 255;
                        aryLightValues[1] = 255;
                        aryLightValues[2] = 255;
                        aryLightValues[3] = 255;
                        aryLightValues[4] = 255;
                        aryLightValues[5] = 255;
                        aryLightValues[6] = 255;
                        aryLightValues[7] = 255;
                    }
                    analogWrite(OUTPUT01_PIN, aryLightValues[0]);
                    analogWrite(OUTPUT02_PIN, aryLightValues[1]);
                    analogWrite(OUTPUT03_PIN, aryLightValues[2]);
                    analogWrite(OUTPUT04_PIN, aryLightValues[3]);
                    analogWrite(OUTPUT05_PIN, aryLightValues[4]);
                    analogWrite(OUTPUT06_PIN, aryLightValues[5]);
                    analogWrite(OUTPUT07_PIN, aryLightValues[6]);
                    analogWrite(OUTPUT08_PIN, aryLightValues[7]);
                }
            }
            else if (message.identifier == 21)
            {
                /* These messages indicate a value of 0 - 255 for the brightness level requested */
                if (message.data[0] == 0)
                {
                    aryLightValues[0] = message.data[1];
                    analogWrite(OUTPUT01_PIN, aryLightValues[0]);
                }
                else if (message.data[0] == 1)
                {
                    aryLightValues[1] = message.data[1];
                    analogWrite(OUTPUT02_PIN, aryLightValues[1]);
                }
                else if (message.data[0] == 2)
                {
                    aryLightValues[2] = message.data[1];
                    analogWrite(OUTPUT03_PIN, aryLightValues[2]);
                }
                else if (message.data[0] == 3)
                {
                    aryLightValues[3] = message.data[1];
                    analogWrite(OUTPUT04_PIN, aryLightValues[3]);
                }
                else if (message.data[0] == 4)
                {
                    aryLightValues[4] = message.data[1];
                    analogWrite(OUTPUT05_PIN, aryLightValues[4]);
                }
                else if (message.data[0] == 5)
                {
                    aryLightValues[5] = message.data[1];
                    analogWrite(OUTPUT06_PIN, aryLightValues[5]);
                }
                else if (message.data[0] == 6)
                {
                    aryLightValues[6] = message.data[1];
                    analogWrite(OUTPUT07_PIN, aryLightValues[6]);
                }
                else if (message.data[0] == 7)
                {
                    aryLightValues[7] = message.data[1];
                    analogWrite(OUTPUT08_PIN, aryLightValues[7]);
                }
            }
            else if (message.identifier == 30)
            {
                if (message.data[0] == 0)
                {
                    lightSequences::startInteriorSequnce01();
                } else if (message.data[0] == 1) {
                    lightSequences::startExteriorSequnce01();
                }
            }
        }
    }

    static void handle_tx_result(bool success) {
        debugf("[CAN] TX %s\n", success ? "OK" : "FAILED");
    }

    void setupCan()
    {
        if (TwaiTaskBased::begin((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, 500000, TWAI_MODE_NO_ACK)) {
            debugln("[CAN] Driver initialized");
        } else {
            debugln("[CAN] Failed to initialize driver");
            return;
        }

        TwaiTaskBased::onReceive(handle_rx_message);
        TwaiTaskBased::onTransmit(handle_tx_result);
        debugln("[CAN] RX/TX callbacks registered");
    }

    void send_status_message()
    {
        // Rate-limit status transmissions
        static unsigned long lastSend = 0;
        unsigned long now = millis();
        if (now - lastSend < STATUS_TX_INTERVAL_MS) return;
        lastSend = now;

        // Configure message to transmit
        twai_message_t message;
        message.identifier = CAN_SEND_MESSAGE_ID;
        message.extd = false;
        message.rtr = false;
        message.data_length_code = 8;
        message.data[0] = aryLightValues[0];
        message.data[1] = aryLightValues[1];
        message.data[2] = aryLightValues[2];
        message.data[3] = aryLightValues[3];
        message.data[4] = aryLightValues[4];
        message.data[5] = aryLightValues[5];
        message.data[6] = aryLightValues[6];
        message.data[7] = aryLightValues[7];

        // Queue message for transmission via background TX task
        TwaiTaskBased::send(message, pdMS_TO_TICKS(10));
    }

    void canLoop()
    {
        // RX is handled asynchronously by TwaiTaskBased FreeRTOS task
        // Only periodic housekeeping needed here
        wifiConfig::checkTimeout();

        // Periodic heartbeat so serial monitor shows the system is alive
        static unsigned long lastHeartbeat = 0;
        unsigned long now = millis();
        if (now - lastHeartbeat >= 5000) {
            lastHeartbeat = now;
            debugf("[CAN] Heartbeat - uptime: %lus, lights: [%d,%d,%d,%d,%d,%d,%d,%d]\n",
                   now / 1000,
                   aryLightValues[0], aryLightValues[1], aryLightValues[2], aryLightValues[3],
                   aryLightValues[4], aryLightValues[5], aryLightValues[6], aryLightValues[7]);
        }
    }
}
