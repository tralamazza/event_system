#ifndef DELEGATE_H
#define DELEGATE_H
#include <functional>
template <typename Func>
class Delegate {
 public:
  Delegate(Func func) : func_{func} {}
  template <typename... Args>
  decltype(auto) operator()(Args&&... args) {
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
    return std::invoke(func_, std::forward<Args>(args)...);
  }

 private:
  Func func_;
};
#endif