#pragma once
#include <cstddef>
#include <functional>
#include <stdint.h>
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef float f32;
typedef double f64;

namespace stdj {
  // go style defer macro
  // https://www.gingerbill.org/article/2015/08/19/defer-in-cpp/
  template<typename F>
  struct privDefer {
    F f;

    privDefer(F f) : f(f) {
    }

    ~privDefer() { f(); }
  };

  template<typename F>
  privDefer<F> defer_func(F f) { return privDefer<F>(f); }

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x) DEFER_2(x, __COUNTER__)
#define defer(code) auto DEFER_3(_defer_) = defer_func([&]() { code; })

  // Source - https://stackoverflow.com/a
  template<typename T>
  int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }

  // template <typename T> int length_of(T x[]) { return sizeof(x) / sizeof(x[0]);
  // }
  // f64 clamp(f64 d, f64 min, f64 max) {
  //   const f64 t = d < min ? min : d;
  //   return t > max ? max : t;
  // }

  // f32 clamp(f32 d, f32 min, f32 max) {
  //   const f32 t = d < min ? min : d;
  //   return t > max ? max : t;
  // }
} // namespace stdj
