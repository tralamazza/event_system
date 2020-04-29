#include <delegate.h>
#include <any>
#include <vector>
#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H
using EventId = size_t;

enum class Event {
  kAnimationNotification,
  kSoftwareInterrupt,
  kHardwareInterrupt,
  kTimeoutNotificaton
};

using FunctionA = std::function<void()>;
using FunctionT = std::function<void(std::int64_t)>;
using FunctionH = std::function<void(const std::int64_t&)>;
using FunctionS = std::function<void(std::int16_t)>;
class EventSystem {
 public:
  void RegisterAnimationNotification(const FunctionA& func);
  void RegisterTimeoutNotification(const FunctionT& func);
  void RegisterHarwardInterrupt(const FunctionH& func);
  void RegisterSoftwareInterrupt(const FunctionS& func);
  void ExecuteAll(const Event& event, const std::any& value = nullptr);
  void Execute(const Event& event, const EventId& event_id,
               const std::any& value = nullptr);

 private:
  std::vector<Delegate<std::function<void()>>> animation_events;
  std::vector<Delegate<std::function<void(std::int64_t)>>> timeout_events;
  std::vector<Delegate<std::function<void(const std::int64_t&)>>>
      hardware_interrupts;
  std::vector<Delegate<std::function<void(std::int16_t)>>> software_interrupts;
};
#endif