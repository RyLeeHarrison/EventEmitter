# EventEmitter for Arduino
EventEmitter is a **lightweight**, **memory‑efficient** event emitter library for Arduino. It brings the familiar **Node.js** event model to embedded systems, allowing you to attach callbacks to events, emit events with custom arguments, and manage one‑time or recurring events—**all without dynamic memory allocation!**

## Features

- **Event Registration:** Attach multiple callbacks to an event.
- **Event Emission:** Emit events and pass arguments to callbacks.
- **One‑Shot Listeners:** Automatically remove listeners after their first call.
- **Listener Management:** Remove individual or all listeners.
- **Memory Efficiency:** Uses fixed‑size buffers and statically allocated arrays, ideal for microcontrollers.

## Using EventEmitter

Below are simple snippet examples to help you get started.

### Registering an Event Listener

Attach a callback that will be called whenever the event is triggered.

```cpp
// Define a simple event handler.
void onMyEvent() {
  // Insert code to run when "myEvent" occurs.
}

// Register the "myEvent" event.
emitter.on("myEvent", onMyEvent);
```

### Emitting an Event

Trigger the event, with or without arguments.

```cpp
// Emit "myEvent" with no arguments.
emitter.emit("myEvent");

// Emit an event with arguments (for example, an integer and a float).
emitter.emit("dataEvent", 42, 3.14);
```

### Using One-Shot Listeners

Register a listener that automatically removes itself after the first call.

```cpp
// The lambda will be executed only once when "startup" is emitted.
emitter.once("startup", [](){
  // Code that runs just once.
});
```

### Removing a Specific Listener

Remove a previously registered listener.

```cpp
// Remove the onMyEvent callback from "myEvent".
emitter.removeListener("myEvent", onMyEvent);
```

### Removing All Listeners

Clear all callbacks for a specific event or for all events.

```cpp
// Remove all listeners from "myEvent".
emitter.removeAllListeners("myEvent");

// Remove every listener from all events.
emitter.removeAllListeners();
```

### Adjusting Maximum Listeners

Limit or retrieve the maximum number of listeners that can be registered per event.

```cpp
// Set the maximum number of listeners per event to 3.
emitter.setMaxListeners(3);

// Retrieve the current maximum listener count.
size_t maxListeners = emitter.getMaxListeners();
```

## License

This project is licensed under the MIT License.
