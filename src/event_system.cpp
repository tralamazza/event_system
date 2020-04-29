#include <../include/event_system.h>
void EventSystem::RegisterAnimationNotification(
    const std::function<void()>& func) {
  animation_events.push_back(Delegate(func));
}

void EventSystem::RegisterTimeoutNotification(
    const std::function<void(std::int64_t)>& func) {
  timeout_events.push_back(Delegate(func));
}

void EventSystem::RegisterHarwardInterrupt(
    const std::function<void(const std::int64_t&)>& func) {
  hardware_interrupts.push_back(Delegate(func));
}

void EventSystem::RegisterSoftwareInterrupt(
    const std::function<void(std::int16_t)>& func) {
  software_interrupts.push_back(Delegate(func));
}

void EventSystem::ExecuteAll(const Event& event, const std::any& value) {
  switch (event) {
    case Event::kAnimationNotification: {
      for (auto& execute : animation_events) {
        execute();
      }
      break;
    }
    case Event::kTimeoutNotificaton: {
      for (auto& execute : timeout_events) {
        execute(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kHardwareInterrupt: {
      for (auto& execute : hardware_interrupts) {
        execute(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kSoftwareInterrupt: {
      for (auto& execute : software_interrupts) {
        execute(std::any_cast<std::int16_t>(value));
      }
      break;
    }
    default: { break; }
  }
}
void EventSystem::Execute(const Event& event, const EventId& event_id,
                          const std::any& value) {
  switch (event) {
    case Event::kAnimationNotification: {
      if (event_id < animation_events.size()) {
        animation_events.at(event_id)();
      }
      break;
    }
    case Event::kTimeoutNotificaton: {
      if (event_id < timeout_events.size()) {
        timeout_events.at(event_id)(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kHardwareInterrupt: {
      if (event_id < hardware_interrupts.size()) {
        hardware_interrupts.at(event_id)(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kSoftwareInterrupt: {
      if (event_id < software_interrupts.size()) {
        software_interrupts.at(event_id)(std::any_cast<std::int16_t>(value));
      }
      break;
    }
    default: { break; }
  }
}