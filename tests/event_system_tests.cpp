/*
 * Copyright 2020 Alexander Dahmen
 */
#include <eventsystem/Dispatcher.hpp>
#include <eventsystem/Event.hpp>
#include <gtest/gtest.h>

using eventsystem::Dispatcher;
using eventsystem::Event;

bool standalone_function_called = false;
bool method_called = false;

class TouchEvent : public Event {
 public:
  static constexpr EventType event_name = "TouchEvent";
  EventType name() const { return event_name; }
};

class Observer {
 public:
  void handleEvent(const Event &event) { method_called = !method_called; }
};

void standalone_function(const Event &event) {
  standalone_function_called = !standalone_function_called;
}

TEST(EventSystemTest, registerUnregisterTest) {
  Dispatcher dispatcher;
  TouchEvent touch_event;
  standalone_function_called = false;

  // test register standalone function
  std::uint8_t id =
      dispatcher.registerCallback(touch_event.name(), standalone_function);
  ASSERT_FALSE(standalone_function_called);
  dispatcher.trigger(touch_event);
  ASSERT_TRUE(standalone_function_called);
  // test unregister standalone function
  dispatcher.unregisterCallback(id);
  dispatcher.trigger(touch_event);
  // should be still true since the standaline function was unregistered
  ASSERT_TRUE(standalone_function_called);
}

TEST(EventSystemTest, methodCallbackTest) {
  Dispatcher dispatcher;
  TouchEvent touch_event;
  Observer observer;

  method_called = false;
  std::uint8_t id = dispatcher.registerCallback(
      touch_event.name(),
      std::bind(&Observer::handleEvent, observer, std::placeholders::_1));
  ASSERT_FALSE(method_called);
  dispatcher.trigger(touch_event);
  ASSERT_TRUE(method_called);
  dispatcher.unregisterCallback(id);
  dispatcher.trigger(touch_event);
  ASSERT_TRUE(method_called);
}
