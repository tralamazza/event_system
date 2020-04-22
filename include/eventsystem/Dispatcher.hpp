/*
 * Copyright 2020 Alexander Dahmen
 */
#ifndef EVENTSYSTEM_DISPATCHER_HPP_
#define EVENTSYSTEM_DISPATCHER_HPP_

#include <algorithm>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "Event.hpp"

namespace eventsystem {
/*
 * Dispatcher class
 * This class is responsible for dispatching events
 * to the corresponding observers.
 *
 * Observers can register callback functions for
 * events.
 */
class Dispatcher {
 public:
  using Callback = std::function<void(const Event &)>;
  /*
   * The CallbackHandle struct safes an id besides the callback.
   * This id is needed in order to unregister a callback from
   * the dispatcher.
   */
  struct CallbackHandle {
    std::uint8_t id;
    Callback callback;
  };
  /*
   * The registerCallback method registers a callback for a special
   * event.
   *
   * Returns an id which is used to unregister the callback from the
   * dispatcher.
   */
  unsigned int registerCallback(const Event::EventType &event_name,
                                Callback &&callback);

  /*
   * The unregisterCallback method deletes the callback to the given id
   * from the observers list of the dispatcher.
   */
  void unregisterCallback(std::uint8_t id);

  /*
   * Via trigger an event can be triggered.
   * All observers for this event will be informed.
   */
  void trigger(const Event &event);

 private:
  // There can be more than one registration for one event (1:N relation)
  std::map<Event::EventType, std::vector<CallbackHandle>> observers_;
  std::uint8_t next_id_ = 0;
  std::mutex mutex_;
};

unsigned int Dispatcher::registerCallback(const Event::EventType &event_name,
                                          Callback &&callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  CallbackHandle callbackHandle = {next_id_, callback};
  observers_[event_name].push_back(callbackHandle);
  return next_id_++;
}

void Dispatcher::unregisterCallback(std::uint8_t id) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto &&observer : observers_) {
    auto &&callback_handles = observer.second;

    // delete callback handle which matches the given id
    callback_handles.erase(
        std::remove_if(callback_handles.begin(), callback_handles.end(),
                       [&](CallbackHandle &handle) { return handle.id == id; }),
        callback_handles.end());
  }
}

void Dispatcher::trigger(const Event &event) {
  std::lock_guard<std::mutex> lock(mutex_);
  // get the name of the event
  auto name = event.name();

  // if there is no event with that name - return
  if (observers_.find(name) == observers_.end())
    return;

  // get registered callback functions and call them
  auto &&callback_handles = observers_.at(name);
  for (auto &&callback_handle : callback_handles) {
    callback_handle.callback(event);
  }
}

}  // namespace eventsystem

#endif  // EVENTSYSTEM_DISPATCHER_HPP_
