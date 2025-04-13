//
// Utility.h
//
// author: Ruan Huipeng
// date : 2025-04-13
//

#ifndef ROC_BASE_UTIL_UTILITY_H
#define ROC_BASE_UTIL_UTILITY_H

#include "base/noncopyable.h"
#include <concepts>
#include <utility>
namespace roc::base::util {

template <typename F>
    requires std::invocable<F>
class defer : public roc::base::noncopyable {
  public:
    explicit defer(const F &func) : func_(func) {}
    ~defer() {
        if (func_) { func_(); }
    }

  private:
    F func_;
};

template<typename F, typename... Args>
    requires std::invocable<F, Args...>
void SAFE_INVOKE(const F &func, Args &&...args) {
    if (func) {
        func(std::forward<Args>(args)...);
    }
}

} // namespace roc::base::util

#endif // ROC_BASE_UTIL_UTILITY_H
