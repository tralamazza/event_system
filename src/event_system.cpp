#include <../include/event_system.h>
void EventSystem::RegisterAnimationNotification(const FunctionA& func) {
  animation_events_.push_back(Delegate(func));
}

void EventSystem::RegisterTimeoutNotification(const FunctionT& func) {
  timeout_events_.push_back(Delegate(func));
}

void EventSystem::RegisterHarwardInterrupt(const FunctionH& func) {
  hardware_interrupts_.push_back(Delegate(func));
}

void EventSystem::RegisterSoftwareInterrupt(const FunctionS& func) {
  software_interrupts_.push_back(Delegate(func));
}

void EventSystem::ExecuteAll(const Event& event, const std::any& value) {
  switch (event) {
    case Event::kAnimationNotification: {
      for (auto& execute : animation_events_) {
        execute();
      }
      break;
    }
    case Event::kTimeoutNotificaton: {
      for (auto& execute : timeout_events_) {
        execute(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kHardwareInterrupt: {
      for (auto& execute : hardware_interrupts_) {
        execute(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kSoftwareInterrupt: {
      for (auto& execute : software_interrupts_) {
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
      if (event_id < animation_events_.size()) {
        animation_events_.at(event_id)();
      }
      break;
    }
    case Event::kTimeoutNotificaton: {
      if (event_id < timeout_events_.size()) {
        timeout_events_.at(event_id)(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kHardwareInterrupt: {
      if (event_id < hardware_interrupts_.size()) {
        hardware_interrupts_.at(event_id)(std::any_cast<std::int64_t>(value));
      }
      break;
    }
    case Event::kSoftwareInterrupt: {
      if (event_id < software_interrupts_.size()) {
        software_interrupts_.at(event_id)(std::any_cast<std::int16_t>(value));
      }
      break;
    }
    default: { break; }
  }
}