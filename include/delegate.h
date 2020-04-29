#ifndef DELEGATE_H
#define DELEGATE_H
#include <functional>
template <typename Func>
class Delegate {
 public:
  explicit Delegate(Func func) : func_{func} {}
  Delegate(const Delegate&) = delete;
  Delegate(Delegate&&) = default;
  Delegate& operator=(Delegate&&) = delete;
  Delegate& operator=(const Delegate&) = delete;
  template <typename... Args>
  decltype(auto) operator()(Args&&... args) {
    return std::invoke(func_, std::forward<Args>(args)...);
  }

 private:
  Func func_;
};
#endif