//
// noncopyable.h
//
// author: Ruan Huipeng
// date : 2025-04-13
//

#ifndef ROC_BASE_NONCOPYABLE_H
#define ROC_BASE_NONCOPYABLE_H

namespace roc::base {

class noncopyable {
  public:
    noncopyable(const noncopyable &) = delete;            // 禁止拷贝构造
    noncopyable &operator=(const noncopyable &) = delete; // 禁止拷贝赋值

    noncopyable() = default;  // 默认构造函数
    ~noncopyable() = default; // 默认析构函数

  private:
};

} // namespace roc::base

#endif // ROC_BASE_NONCOPYABLE_H
