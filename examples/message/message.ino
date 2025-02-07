#include <Arduino.h>
#include "EventEmitter.hpp"

// Create an EventEmitter instance.
EventEmitter<> emitter;

// Callback that receives a message and a count.
void messageHandler(const char* msg, int count) {
  Serial.print("Message: ");
  Serial.print(msg);
  Serial.print(" | Count: ");
  Serial.println(count);
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for the serial port to connect.

  // Register the "message" event with a callback expecting (const char*, int).
  emitter.on("message", messageHandler);
}

void loop() {
  // Emit the "message" event with two arguments.
  emitter.emit("message", "Hello, Arduino!", 1);
  delay(1000);
}
