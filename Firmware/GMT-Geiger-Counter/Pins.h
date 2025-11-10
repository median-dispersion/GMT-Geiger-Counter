#ifndef PINS_H
#define PINS_H

#include "Arduino.h"
#include "Configuration.h"

void setupPins() {

    // Set pin modes
    // -------------

    // SPI Pins
    pinMode(DISPLAY_CS_PIN, OUTPUT);
    pinMode(TOUCH_CS_PIN, OUTPUT);
    pinMode(SD_CS_PIN, OUTPUT);
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);

    // Display pins
    pinMode(DISPLAY_LED_PIN, OUTPUT);
    pinMode(DISPLAY_RST_PIN, OUTPUT);
    pinMode(DISPLAY_DC_PIN, OUTPUT);

    // Touchscreen pins
    pinMode(TOUCH_IRQ_PIN, INPUT);

    // Tube pins
    pinMode(MAIN_TRG_PIN, INPUT);
    pinMode(FOLLOWER_TRG_PIN, INPUT);
    pinMode(COINCIDENCE_TRG_PIN, INPUT);

    // Other pins
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RGB_LED_PIN, OUTPUT);

    // Set pin states
    // --------------

    // SPI Pins
    digitalWrite(DISPLAY_CS_PIN, HIGH);
    digitalWrite(TOUCH_CS_PIN, HIGH);
    digitalWrite(SD_CS_PIN, HIGH);
    digitalWrite(SCK_PIN, LOW);
    digitalWrite(MOSI_PIN, LOW);

    // Display pins
    digitalWrite(DISPLAY_LED_PIN, LOW);
    digitalWrite(DISPLAY_RST_PIN, HIGH);
    digitalWrite(DISPLAY_DC_PIN, HIGH);

    // Other pins
    pinMode(BUZZER_PIN, LOW);
    pinMode(RGB_LED_PIN, LOW);

}

#endif