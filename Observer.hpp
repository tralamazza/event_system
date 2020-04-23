/*
 * Copyright 2020 Alexander Dahmen
 */
#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include <iostream>

#include "BufferFullEvent.hpp"
#include "TestEvent.hpp"
#include <eventsystem/Event.hpp>

using eventsystem::Event;

/*
 * Simple Observer class to demonstrate receiving events
 */
class Observer {
 public:
  void handleEvent(const Event &event) {
    if (event.name() == TestEvent::event_name) {
      std::cout << "TestEvent received!" << std::endl;
    } else if (event.name() == BufferFullEvent::event_name) {
      std::cout << "BufferFullEvent received!" << std::endl;
    }
  }
};

#endif  // OBSERVER_HPP_
