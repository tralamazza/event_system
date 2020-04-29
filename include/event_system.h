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

using functionA = std::function<void()>;
using functionT = std::function<void(std::int64_t)>;
using functionH = std::function<void(const std::int64_t&)>;
using functionS = std::function<void(std::int16_t)>;
class EventSystem {
 public:
  void RegisterAnimationNotification(const std::function<void()>& func);
  void RegisterTimeoutNotification(
      const std::function<void(std::int64_t)>& func);
  void RegisterHarwardInterrupt(
      const std::function<void(const std::int64_t&)>& func);
  void RegisterSoftwareInterrupt(const std::function<void(std::int16_t)>& func);
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