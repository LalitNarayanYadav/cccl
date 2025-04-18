//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <cuda/std/chrono>

// system_clock

// time_t to_time_t(const time_point& t);

#include <cuda/std/chrono>
#include <cuda/std/ctime>

int main(int, char**)
{
  typedef cuda::std::chrono::system_clock C;
  [[maybe_unused]] cuda::std::time_t t1 = C::to_time_t(C::now());

  return 0;
}
