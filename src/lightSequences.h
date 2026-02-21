#pragma once
#include <Arduino.h>
#include "globals.h"

namespace lightSequences
{
    void startupLightShow()
    {
        // All lights off first
        analogWrite(OUTPUT01_PIN, 0);
        analogWrite(OUTPUT02_PIN, 0);
        analogWrite(OUTPUT03_PIN, 0);
        analogWrite(OUTPUT04_PIN, 0);
        analogWrite(OUTPUT05_PIN, 0);
        analogWrite(OUTPUT06_PIN, 0);
        analogWrite(OUTPUT07_PIN, 0);
        analogWrite(OUTPUT08_PIN, 0);
        delay(500);

        // ===== SECTION 1: Progressive Wave (5 seconds) =====
        // Wave across all 8 lights from left to right
        for (int wave = 0; wave < 3; wave++)
        {
            for (int i = 0; i < 8; i++)
            {
                int pins[8] = {OUTPUT01_PIN, OUTPUT02_PIN, OUTPUT03_PIN, OUTPUT04_PIN,
                               OUTPUT05_PIN, OUTPUT06_PIN, OUTPUT07_PIN, OUTPUT08_PIN};

                // Turn on current light
                analogWrite(pins[i], 255);
                delay(100);

                // Turn off current light
                analogWrite(pins[i], 0);
            }
        }
        delay(300);

        // ===== SECTION 2: All Lights Pulse Together (4 seconds) =====
        for (int pulse = 0; pulse < 4; pulse++)
        {
            // Fade in
            for (int i = 0; i <= 255; i += 25)
            {
                analogWrite(OUTPUT01_PIN, i);
                analogWrite(OUTPUT02_PIN, i);
                analogWrite(OUTPUT03_PIN, i);
                analogWrite(OUTPUT04_PIN, i);
                analogWrite(OUTPUT05_PIN, i);
                analogWrite(OUTPUT06_PIN, i);
                analogWrite(OUTPUT07_PIN, i);
                analogWrite(OUTPUT08_PIN, i);
                delay(30);
            }
            // Fade out
            for (int i = 255; i >= 0; i -= 25)
            {
                analogWrite(OUTPUT01_PIN, i);
                analogWrite(OUTPUT02_PIN, i);
                analogWrite(OUTPUT03_PIN, i);
                analogWrite(OUTPUT04_PIN, i);
                analogWrite(OUTPUT05_PIN, i);
                analogWrite(OUTPUT06_PIN, i);
                analogWrite(OUTPUT07_PIN, i);
                analogWrite(OUTPUT08_PIN, i);
                delay(30);
            }
        }
        delay(300);

        // ===== SECTION 3: Alternating Pattern (5 seconds) =====
        for (int alt = 0; alt < 5; alt++)
        {
            // Odd lights on
            analogWrite(OUTPUT01_PIN, 255);
            analogWrite(OUTPUT03_PIN, 255);
            analogWrite(OUTPUT05_PIN, 255);
            analogWrite(OUTPUT07_PIN, 255);
            delay(200);

            // Even lights on
            analogWrite(OUTPUT01_PIN, 0);
            analogWrite(OUTPUT03_PIN, 0);
            analogWrite(OUTPUT05_PIN, 0);
            analogWrite(OUTPUT07_PIN, 0);

            analogWrite(OUTPUT02_PIN, 255);
            analogWrite(OUTPUT04_PIN, 255);
            analogWrite(OUTPUT06_PIN, 255);
            analogWrite(OUTPUT08_PIN, 255);
            delay(200);

            // All off
            analogWrite(OUTPUT02_PIN, 0);
            analogWrite(OUTPUT04_PIN, 0);
            analogWrite(OUTPUT06_PIN, 0);
            analogWrite(OUTPUT08_PIN, 0);
            delay(100);
        }
        delay(300);

        // ===== SECTION 4: Spiral Pattern (6 seconds) =====
        // Lights spiral inward from edges
        for (int spiral = 0; spiral < 3; spiral++)
        {
            int pins[8] = {OUTPUT01_PIN, OUTPUT02_PIN, OUTPUT03_PIN, OUTPUT04_PIN,
                           OUTPUT05_PIN, OUTPUT06_PIN, OUTPUT07_PIN, OUTPUT08_PIN};

            // Forward spiral
            for (int i = 0; i < 8; i++)
            {
                analogWrite(pins[i], 255);
                delay(80);
            }
            delay(200);

            // Backward spiral with fade
            for (int i = 7; i >= 0; i--)
            {
                analogWrite(pins[i], 0);
                delay(80);
            }
            delay(200);
        }
        delay(300);

        // ===== SECTION 5: Finale - All Lights Rapid Flash (4 seconds) =====
        for (int flash = 0; flash < 8; flash++)
        {
            // All on
            analogWrite(OUTPUT01_PIN, 255);
            analogWrite(OUTPUT02_PIN, 255);
            analogWrite(OUTPUT03_PIN, 255);
            analogWrite(OUTPUT04_PIN, 255);
            analogWrite(OUTPUT05_PIN, 255);
            analogWrite(OUTPUT06_PIN, 255);
            analogWrite(OUTPUT07_PIN, 255);
            analogWrite(OUTPUT08_PIN, 255);
            delay(200);

            // All off
            analogWrite(OUTPUT01_PIN, 0);
            analogWrite(OUTPUT02_PIN, 0);
            analogWrite(OUTPUT03_PIN, 0);
            analogWrite(OUTPUT04_PIN, 0);
            analogWrite(OUTPUT05_PIN, 0);
            analogWrite(OUTPUT06_PIN, 0);
            analogWrite(OUTPUT07_PIN, 0);
            analogWrite(OUTPUT08_PIN, 0);
            delay(200);
        }

        // ===== Ensure all lights are off =====
        analogWrite(OUTPUT01_PIN, 0);
        analogWrite(OUTPUT02_PIN, 0);
        analogWrite(OUTPUT03_PIN, 0);
        analogWrite(OUTPUT04_PIN, 0);
        analogWrite(OUTPUT05_PIN, 0);
        analogWrite(OUTPUT06_PIN, 0);
        analogWrite(OUTPUT07_PIN, 0);
        analogWrite(OUTPUT08_PIN, 0);
    }

    void startInteriorSequnce01()
    {
        analogWrite(OUTPUT08_PIN, 0);
        analogWrite(OUTPUT07_PIN, 0);
        analogWrite(OUTPUT06_PIN, 0);
        analogWrite(OUTPUT05_PIN, 0);
        // Loop from 0 to 255 (increasing)
        for (int i = 0; i <= 255; i++)
        {
            analogWrite(OUTPUT08_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT07_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT06_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT05_PIN, i); // Set the PWM value to i
            delay(10);                    // Wait for 10 milliseconds before the next step
        }

        // Loop from 255 to 0 (decreasing)
        for (int i = 255; i >= 0; i--)
        {
            analogWrite(OUTPUT08_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT07_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT06_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT05_PIN, i); // Set the PWM value to i
            delay(10);                    // Wait for 10 milliseconds before the next step
        }

        for (int i = 0; i <= 30; i++)
        {
            analogWrite(OUTPUT05_PIN, 0);
            analogWrite(OUTPUT08_PIN, 255);
            analogWrite(OUTPUT07_PIN, 0);
            analogWrite(OUTPUT06_PIN, 0);
            delay(60);

            analogWrite(OUTPUT08_PIN, 0);
            analogWrite(OUTPUT07_PIN, 255);
            analogWrite(OUTPUT06_PIN, 0);
            analogWrite(OUTPUT05_PIN, 0);
            delay(60);

            analogWrite(OUTPUT08_PIN, 0);
            analogWrite(OUTPUT07_PIN, 0);
            analogWrite(OUTPUT06_PIN, 255);
            analogWrite(OUTPUT05_PIN, 0);
            delay(60);

            analogWrite(OUTPUT08_PIN, 0);
            analogWrite(OUTPUT07_PIN, 0);
            analogWrite(OUTPUT06_PIN, 0);
            analogWrite(OUTPUT05_PIN, 255);
            delay(60);

            analogWrite(OUTPUT08_PIN, 0);
            analogWrite(OUTPUT07_PIN, 0);
            analogWrite(OUTPUT06_PIN, 255);
            analogWrite(OUTPUT05_PIN, 0);
            delay(60);

            analogWrite(OUTPUT08_PIN, 0);
            analogWrite(OUTPUT07_PIN, 255);
            analogWrite(OUTPUT06_PIN, 0);
            analogWrite(OUTPUT05_PIN, 0);
            delay(60);

            analogWrite(OUTPUT08_PIN, 255);
            analogWrite(OUTPUT07_PIN, 0);
            analogWrite(OUTPUT06_PIN, 0);
            analogWrite(OUTPUT05_PIN, 0);
            delay(60);
        }
        analogWrite(OUTPUT08_PIN, 0);
        analogWrite(OUTPUT07_PIN, 0);
        analogWrite(OUTPUT06_PIN, 0);
        analogWrite(OUTPUT05_PIN, 0);
    }

    void startExteriorSequnce01()
    {
        analogWrite(OUTPUT04_PIN, 0);
        analogWrite(OUTPUT03_PIN, 0);
        // Loop from 0 to 255 (increasing)
        for (int i = 0; i <= 255; i++)
        {
            analogWrite(OUTPUT04_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT03_PIN, i); // Set the PWM value to i
            delay(10);                    // Wait for 10 milliseconds before the next step
        }

        // Loop from 255 to 0 (decreasing)
        for (int i = 255; i >= 0; i--)
        {
            analogWrite(OUTPUT04_PIN, i); // Set the PWM value to i
            analogWrite(OUTPUT03_PIN, i); // Set the PWM value to i
            delay(10);                    // Wait for 10 milliseconds before the next step
        }

        for (int i = 0; i <= 30; i++)
        {
            analogWrite(OUTPUT04_PIN, 0);
            analogWrite(OUTPUT03_PIN, 255);
            delay(250);

            analogWrite(OUTPUT04_PIN, 255);
            analogWrite(OUTPUT03_PIN, 0);
            delay(250);
        }
        analogWrite(OUTPUT04_PIN, 0);
        analogWrite(OUTPUT03_PIN, 0);
    }    
}