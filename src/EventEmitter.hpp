//
//    FILE: EventEmitter.hpp
//  AUTHOR: RyLee Harrison
// VERSION: 1.0.0
//     URL: https://github.com/RyLeeHarrison/EventEmitter

#ifndef EVENTEMITTER_HPP
#define EVENTEMITTER_HPP

#include <cstddef>
#include <cstring>
#include <array>

// Define a callback type alias which can accept any arguments.
template<typename... Args>
using EventCallback = void (*)(Args...);

template<size_t MaxEvents = 10, size_t MaxListeners = 5, size_t MaxEventNameLength = 16>
class EventEmitter {
private:
    struct Listener {
        void* callback;
        bool once;
        bool active;
    };

    struct Event {
        char name[MaxEventNameLength];
        std::array<Listener, MaxListeners> listeners;
        size_t listenerCount;
        bool active;
    };

    std::array<Event, MaxEvents> events;
    size_t eventCount;
    size_t maxListeners;

    // Compare names using strncmp for our fixed-length names.
    Event* findEvent(const char* name) {
        for (size_t i = 0; i < eventCount; i++) {
            if (events[i].active && (std::strncmp(events[i].name, name, MaxEventNameLength) == 0)) {
                return &events[i];
            }
        }
        return nullptr;
    }

    // Create a new event. Returns nullptr if the event pool is full.
    Event* createEvent(const char* name) {
        if (eventCount >= MaxEvents)
            return nullptr;
        Event& ev = events[eventCount];
        std::strncpy(ev.name, name, MaxEventNameLength - 1);
        ev.name[MaxEventNameLength - 1] = '\0'; // Ensure null termination.
        ev.listenerCount = 0;
        ev.active = true;
        return &events[eventCount++];
    }

public:
    EventEmitter() : eventCount(0), maxListeners(MaxListeners) {}

    // Register a listener (callback) for an event.
    template<typename... Args>
    EventEmitter& on(const char* eventName, EventCallback<Args...> callback) {
        Event* event = findEvent(eventName);
        if (!event) {
            event = createEvent(eventName);
            if (!event)
                return *this;
        }
        if (event->listenerCount < maxListeners) {
            event->listeners[event->listenerCount] = { reinterpret_cast<void*>(callback), false, true };
            event->listenerCount++;
        }
        return *this;
    }

    // Emit an event, passing the provided arguments to each callback.
    template<typename... Args>
    bool emit(const char* eventName, Args... args) {
        Event* event = findEvent(eventName);
        if (!event)
            return false;

        size_t activeCount = 0;
        for (size_t i = 0; i < event->listenerCount; ++i) {
            if (event->listeners[i].active) {
                auto callback = reinterpret_cast<EventCallback<Args...>>(event->listeners[i].callback);
                callback(args...);
                if (event->listeners[i].once) {
                    event->listeners[i].active = false;
                } else {
                    if (i != activeCount) {
                        event->listeners[activeCount] = event->listeners[i];
                    }
                    activeCount++;
                }
            }
        }
        event->listenerCount = activeCount;
        if (activeCount == 0) {
            event->active = false;
        }
        return true;
    }

    // Remove a specific listener for an event.
    template<typename... Args>
    EventEmitter& removeListener(const char* eventName, EventCallback<Args...> callback) {
        Event* event = findEvent(eventName);
        if (!event)
            return *this;
        size_t activeCount = 0;
        for (size_t i = 0; i < event->listenerCount; ++i) {
            if (event->listeners[i].callback != reinterpret_cast<void*>(callback)) {
                if (i != activeCount) {
                    event->listeners[activeCount] = event->listeners[i];
                }
                activeCount++;
            }
        }
        event->listenerCount = activeCount;
        if (activeCount == 0) {
            event->active = false;
        }
        return *this;
    }

    // Remove all listeners, either for a specific event or for all events.
    EventEmitter& removeAllListeners(const char* eventName = nullptr) {
        if (eventName) {
            Event* event = findEvent(eventName);
            if (event) {
                event->listenerCount = 0;
                event->active = false;
            }
        } else {
            eventCount = 0;
            for (size_t i = 0; i < MaxEvents; i++) {
                events[i].active = false;
                events[i].listenerCount = 0;
            }
        }
        return *this;
    }

    // Register a one-shot listener which is automatically removed after it is called.
    template<typename... Args>
    EventEmitter& once(const char* eventName, EventCallback<Args...> callback) {
        Event* event = findEvent(eventName);
        if (!event) {
            event = createEvent(eventName);
            if (!event) return *this;
        }
        if (event->listenerCount < maxListeners) {
            event->listeners[event->listenerCount] = { reinterpret_cast<void*>(callback), true, true };
            event->listenerCount++;
        }
        return *this;
    }

    // Set (or limit) the maximum number of listeners per event.
    EventEmitter& setMaxListeners(size_t n) {
        maxListeners = (n > MaxListeners) ? MaxListeners : n;
        return *this;
    }

    size_t getMaxListeners() const {
        return maxListeners;
    }

    static const size_t defaultMaxListeners = MaxListeners;
};

#endif  // EVENTEMITTER_HPP
