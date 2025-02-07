#include <Arduino.h>
#include "EventEmitter.hpp"

// Create an EventEmitter instance.
EventEmitter<> emitter;

// One-shot callback prints a message.
void oneShotHandler() {
  Serial.println("This one-shot message appears only once!");
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for the serial port to connect.

  // Register the "oneshot" event with a one-shot listener.
  emitter.once("oneshot", oneShotHandler);
}

void loop() {
  // Try emitting the "oneshot" event repeatedly.
  // The callback will run only the first time.
  emitter.emit("oneshot");
  delay(1000);
}
