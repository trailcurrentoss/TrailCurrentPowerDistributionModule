#pragma once
#include "globals.h"
#include "driver/twai.h"
#include "lightSequences.h"
#include <OtaUpdate.h>
#include "Secrets.h"
#include "wifiConfig.h"

// Forward declare otaUpdate (defined in main.cpp)
extern OtaUpdate otaUpdate;

#define CAN_RX 13
#define CAN_TX 15
#define CAN_SEND_MESSAGE_ID 0x1B;
// Interval:
#define POLLING_RATE_MS 33
static bool driver_installed = false;
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

    void setupCan()
    {
        // Initialize configuration structures using macro initializers
        twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NO_ACK);
        twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); // Look in the api-reference for other speed sets.
        // Accept all message IDs (0x0 OTA, 0x1 WiFi config, 0x15/0x21 brightness, 0x18 toggle, 0x1E sequences, 0x1B status)
        twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

        // Install TWAI driver
        if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
        {
            debugln("Driver installed");
        }
        else
        {
            debugln("Failed to install driver");
            return;
        }

        // Start TWAI driver
        if (twai_start() == ESP_OK)
        {
            debugln("Driver started");
        }
        else
        {
            debugln("Failed to start driver");
            return;
        }

        // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
        uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL;
        if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK)
        {
            debugln("CAN Alerts reconfigured");
        }
        else
        {
            debugln("Failed to reconfigure alerts");
            return;
        }

        // TWAI driver is now successfully installed and started
        driver_installed = true;
    }

    void send_status_message()
    {
        // Send message
        // Configure message to transmit
        twai_message_t message;
        message.identifier = CAN_SEND_MESSAGE_ID;
        message.extd = false; // Using CAN 2.0 extended id allowing up to 536870911 identifiers
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

        // Queue message for transmission
        if (twai_transmit(&message, pdMS_TO_TICKS(10)) == ESP_OK)
        {
            // printf("Message queued for transmission\n");
        }
        else
        {
            printf("Failed to queue message for transmission\n");
        }
    }

    static void handle_rx_message(twai_message_t &message)
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
            else if (message.identifier == 24) // Message ID of 21 are on/off messages
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
                    Serial.println("Got HERE");
                    Serial.println(message.data[1]);
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
                    Serial.println("Got HERE");
                    Serial.println(message.data[1]);
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

    void canLoop()
    {
        if (!driver_installed)
        {
            // Driver not installed
            delay(1000);
            return;
        }
        // Check if alert happened
        uint32_t alerts_triggered;
        twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));
        twai_status_info_t twaistatus;
        twai_get_status_info(&twaistatus);

        // Handle alerts
        if (alerts_triggered & TWAI_ALERT_ERR_PASS)
        {
            debugln("Alert: TWAI controller has become error passive.");
        }
        if (alerts_triggered & TWAI_ALERT_BUS_ERROR)
        {
            debugln("Alert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.");
            debugf("Bus error count: %lu\n", twaistatus.bus_error_count);
        }
        if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL)
        {
            debugln("Alert: The RX queue is full causing a received frame to be lost.");
            debugf("RX buffered: %lu\t", twaistatus.msgs_to_rx);
            debugf("RX missed: %lu\t", twaistatus.rx_missed_count);
            debugf("RX overrun %lu\n", twaistatus.rx_overrun_count);
        }

        // Check if message is received
        if (alerts_triggered & TWAI_ALERT_RX_DATA)
        {
            // One or more messages received. Handle all.
            twai_message_t message;
            while (twai_receive(&message, 0) == ESP_OK)
            {
                handle_rx_message(message);
            }
        }

        // Check for timeout on incomplete WiFi config sequences
        wifiConfig::checkTimeout();
    }
}