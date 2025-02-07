#include <Arduino.h>
#include "EventEmitter.hpp"

// Create an EventEmitter instance.
EventEmitter<> emitter;

void testHandler() {
  Serial.println("Test event triggered");
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for the serial port to connect.
  // Register the "test" event.
  emitter.on("test", testHandler);

  // Emit the event once; you should see the message.
  emitter.emit("test");

  // Now remove the listener.
  emitter.removeListener("test", testHandler);
}

void loop() {
  // Since the listener was removed, nothing will happen.
  emitter.emit("test");
  delay(1000);
}
