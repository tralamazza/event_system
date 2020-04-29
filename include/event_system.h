#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H
#include <delegate.h>
#include <any>
#include <vector>

enum class Event {
  kAnimationNotification,
  kSoftwareInterrupt,
  kHardwareInterrupt,
  kTimeoutNotificaton
};
using EventId = size_t;
using FunctionA = std::function<void()>;
using FunctionT = std::function<void(std::int64_t)>;
using FunctionH = std::function<void(const std::int64_t&)>;
using FunctionS = std::function<void(std::int16_t)>;
class EventSystem {
 public:
  EventSystem() = default;
  EventSystem(const EventSystem&) = delete;
  EventSystem(EventSystem&&) = delete;
  EventSystem& operator=(EventSystem&&) = delete;
  EventSystem& operator=(const EventSystem&) = delete;
  void RegisterAnimationNotification(const FunctionA& func);
  void RegisterTimeoutNotification(const FunctionT& func);
  void RegisterHarwardInterrupt(const FunctionH& func);
  void RegisterSoftwareInterrupt(const FunctionS& func);
  void ExecuteAll(const Event& event, const std::any& value = nullptr);
  void Execute(const Event& event, const EventId& event_id,
               const std::any& value = nullptr);

 private:
  std::vector<Delegate<FunctionA>> animation_events_;
  std::vector<Delegate<FunctionT>> timeout_events_;
  std::vector<Delegate<FunctionH>> hardware_interrupts_;
  std::vector<Delegate<FunctionS>> software_interrupts_;
};
#endif