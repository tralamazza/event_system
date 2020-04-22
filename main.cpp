/*
 * Copyright 2020 Alexander Dahmen
 */
#include <iostream>

#include "Observer.hpp"
#include "TestEvent.hpp"
#include <eventsystem/Dispatcher.hpp>
#include <eventsystem/Event.hpp>

using eventsystem::Dispatcher;
using eventsystem::Event;

/*
 * Client Testprogram
 */
void hello_function(const Event &event) {
  std::cout << "Hello from function" << std::endl;
}

int main(int argc, char *argv[]) {
  Dispatcher dispatcher;
  Observer observer;
  TestEvent test_event;
  BufferFullEvent buffer_full_event;

  // callback object::method
  std::uint8_t id_observer_test = dispatcher.registerCallback(
      test_event.name(),
      std::bind(&Observer::handleEvent, observer, std::placeholders::_1));
  std::uint8_t id_observer_buffer = dispatcher.registerCallback(
      buffer_full_event.name(),
      std::bind(&Observer::handleEvent, observer, std::placeholders::_1));
  // callback stand alone function
  std::uint8_t id_hello_function =
      dispatcher.registerCallback(test_event.name(), hello_function);

  dispatcher.trigger(test_event);
  dispatcher.trigger(buffer_full_event);

  // unregister one of them
  dispatcher.unregisterCallback(id_observer_test);

  dispatcher.trigger(test_event);
  dispatcher.trigger(buffer_full_event);
}
