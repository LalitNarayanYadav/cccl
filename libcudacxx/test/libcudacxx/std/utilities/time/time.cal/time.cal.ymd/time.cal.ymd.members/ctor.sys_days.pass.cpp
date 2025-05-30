//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>
// class year_month_day;

//  constexpr year_month_day(const sys_days& dp) noexcept;
//
//  Effects:  Constructs an object of type year_month_day that corresponds
//                to the date represented by dp.
//
//  Remarks: For any value ymd of type year_month_day for which ymd.ok() is true,
//                ymd == year_month_day{sys_days{ymd}} is true.
//
//  constexpr chrono::year   year() const noexcept;
//  constexpr chrono::month month() const noexcept;
//  constexpr bool             ok() const noexcept;

#include <cuda/std/cassert>
#include <cuda/std/chrono>
#include <cuda/std/type_traits>

#include "test_macros.h"

TEST_DIAG_SUPPRESS_MSVC(4307) // potential overflow
TEST_DIAG_SUPPRESS_MSVC(4308) // unsigned/signed comparisons

int main(int, char**)
{
  using year           = cuda::std::chrono::year;
  using day            = cuda::std::chrono::day;
  using sys_days       = cuda::std::chrono::sys_days;
  using days           = cuda::std::chrono::days;
  using year_month_day = cuda::std::chrono::year_month_day;

  static_assert(noexcept(year_month_day{cuda::std::declval<sys_days>()}));

  auto constexpr January = cuda::std::chrono::January;

  {
    constexpr sys_days sd{};
    constexpr year_month_day ymd{sd};

    static_assert(ymd.ok(), "");
    static_assert(ymd.year() == year{1970}, "");
    static_assert(ymd.month() == January, "");
    static_assert(ymd.day() == day{1}, "");
  }

  {
    constexpr sys_days sd{days{10957 + 32}};
    constexpr year_month_day ymd{sd};

    auto constexpr February = cuda::std::chrono::February;

    static_assert(ymd.ok(), "");
    static_assert(ymd.year() == year{2000}, "");
    static_assert(ymd.month() == February, "");
    static_assert(ymd.day() == day{2}, "");
  }

  //  There's one more leap day between 1/1/40 and 1/1/70
  //  when compared to 1/1/70 -> 1/1/2000
  {
    constexpr sys_days sd{days{-10957}};
    constexpr year_month_day ymd{sd};

    static_assert(ymd.ok(), "");
    static_assert(ymd.year() == year{1940}, "");
    static_assert(ymd.month() == January, "");
    static_assert(ymd.day() == day{2}, "");
  }

  {
    sys_days sd{days{-(10957 + 34)}};
    year_month_day ymd{sd};
    auto constexpr November = cuda::std::chrono::November;

    assert(ymd.ok());
    assert(ymd.year() == year{1939});
    assert(ymd.month() == November);
    assert(ymd.day() == day{29});
  }

  return 0;
}
