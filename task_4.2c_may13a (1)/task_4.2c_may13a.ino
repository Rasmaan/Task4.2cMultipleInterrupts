#include "arduino_secrets.h"
#include <Wire.h>
#include <BH1750FVI.h>

#define BUTTON_PIN 2       // Switch connected to pin 2
#define LED1_PIN 5         // LED1 toggled by switch
#define LED2_PIN 6         // LED2 controlled by light sensor

BH1750FVI LightSensor(BH1750FVI::k_DevModeContHighRes);  // Continuous mode, high resolution

volatile bool isButtonPressed = false;
bool led2State = false;

void onButtonPress() {
  isButtonPressed = true;
}

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);

  Wire.begin();
  LightSensor.begin();
}

void loop() {
  // Handle switch
  if (isButtonPressed) {
    isButtonPressed = false;
    digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
    Serial.println("Switch pressed! LED1 toggled.");
  }

  // Read lux from sensor
  uint16_t lux = LightSensor.GetLightIntensity();

  Serial.print("Light Intensity: ");
  Serial.print(lux);
  Serial.println(" lux");

  // Toggle LED2 based on brightness
  if (lux < 20) {
    if (!led2State) {
      digitalWrite(LED2_PIN, HIGH);
      led2State = true;
      Serial.println("LED2 ON: It's dark.");
    }
  } else {
    if (led2State) {
      digitalWrite(LED2_PIN, LOW);
      led2State = false;
      Serial.println("LED2 OFF: It's bright.");
    }
  }

  delay(500);
}
