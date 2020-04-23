/*
 * Copyright 2020 by Alexander Dahmen
 */
#ifndef EVENTSYSTEM_EVENT_HPP_
#define EVENTSYSTEM_EVENT_HPP_

namespace eventsystem {

class Event {
 public:
  Event() {}
  virtual ~Event() {}
  using EventType = const char *;
  virtual EventType name() const = 0;
};

}  // namespace eventsystem

#endif  // EVENTSYSTEM_EVENT_HPP_
