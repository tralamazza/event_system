#include <delegate.h>
#include <event_system.h>
#include <iostream>
using namespace std;

std::int64_t i = 0;
void Foo() { std::cout << "Foo:" << ++i << endl; }
void Foo_One(std::int64_t x) {
  std::cout << "Foo one argument:" << x << std::endl;
}
struct A {
  void Foo(const std::int64_t& x) { std::cout << "A Foo:" << x << std::endl; }
};
extern "C" void ExtFunction(std::int64_t x) {
  std::cout << "Extern C function:" << x << std::endl;
};
static void Print(std::int16_t s) {
  std::cout << "Static function:" << s << std::endl;
}
int main() {
  cout << "Event based registration..." << endl;
  auto lambda = []() { cout << "Lambda" << endl; };
  EventSystem event_system;
  event_system.RegisterAnimationNotification(std::ref(Foo));
  event_system.RegisterAnimationNotification(std::ref(lambda));
  event_system.Execute(Event::kAnimationNotification, 0);
  event_system.Execute(Event::kAnimationNotification, 1);
  event_system.ExecuteAll(Event::kAnimationNotification);
  auto lambda1 = [](std::int64_t x) { cout << "Lambda:" << x << endl; };
  std::int64_t value{10};
  event_system.RegisterTimeoutNotification(std::ref(Foo_One));
  event_system.RegisterTimeoutNotification(std::ref(lambda1));
  event_system.RegisterTimeoutNotification(std::ref(ExtFunction));
  event_system.ExecuteAll(Event::kTimeoutNotificaton, value);
  event_system.Execute(Event::kTimeoutNotificaton, 1, value + 100);
  event_system.Execute(Event::kTimeoutNotificaton, 2, value + 200);
  A a;
  auto a_foo = std::bind(&A::Foo, &a, std::placeholders::_1);
  event_system.RegisterHarwardInterrupt(std::ref(a_foo));
  event_system.Execute(Event::kHardwareInterrupt, 0, value);
  event_system.RegisterSoftwareInterrupt(std::ref(Print));
  std::int16_t another_value{50};
  event_system.ExecuteAll(Event::kSoftwareInterrupt, another_value);
  return 0;
}