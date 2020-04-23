/*
 * Copyright 2020 Alexander Dahmen
 */
#ifndef TESTEVENT_HPP_
#define TESTEVENT_HPP_

#include <eventsystem/Event.hpp>

/*
 * Simple Test Event for demonstration
 */
class TestEvent : public eventsystem::Event {
 public:
  static constexpr EventType event_name = "TestEvent";
  EventType name() const { return event_name; }
};

#endif  // TESTEVENT_HPP_
