/*
 * Copyright 2020 Alexander Dahmen
 */
#ifndef BUFFERFULLEVENT_HPP_
#define BUFFERFULLEVENT_HPP_

#include <eventsystem/Event.hpp>

/*
 * Another simple Event to demonstrate different Events.
 * Such an event can be extended with more methods and
 * private fields to provide more information.
 */
class BufferFullEvent : public eventsystem::Event {
 public:
  static constexpr EventType event_name = "BufferFullEvent";
  EventType name() const { return event_name; }
};

#endif  // BUFFERFULLEVENT_HPP_
