#include <Arduino.h>
#include "EventEmitter.hpp"

// Create an EventEmitter instance using the default parameters.
EventEmitter<> emitter;

const int ledPin = 13;

// Callback that toggles the LED.
void blinkHandler() {
  digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for the serial port to connect.

  pinMode(ledPin, OUTPUT);
  // Optional: Start the serial port for debugging.
  // Register the "blink" event.
  emitter.on("blink", blinkHandler);
}

void loop() {
  // Emit the "blink" event (no arguments are needed for this callback).
  emitter.emit("blink");
  delay(500);
}
