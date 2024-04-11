//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11
// <cuda/std/optional>

// template <class U>
//   explicit optional(optional<U>&& rhs);

#include <cuda/std/cassert>
#include <cuda/std/optional>
#include <cuda/std/type_traits>

#include "test_macros.h"

using cuda::std::optional;

template <class T, class U>
__host__ __device__ TEST_CONSTEXPR_CXX14 void test(optional<U>&& rhs, bool is_going_to_throw = false)
{
  static_assert(!(cuda::std::is_convertible<optional<U>&&, optional<T>>::value), "");
  bool rhs_engaged = static_cast<bool>(rhs);
#ifndef TEST_HAS_NO_EXCEPTIONS
  try
  {
    optional<T> lhs(cuda::std::move(rhs));
    assert(is_going_to_throw == false);
    assert(static_cast<bool>(lhs) == rhs_engaged);
  }
  catch (int i)
  {
    assert(i == 6);
  }
#else
  if (is_going_to_throw)
  {
    return;
  }
  optional<T> lhs(cuda::std::move(rhs));
  assert(static_cast<bool>(lhs) == rhs_engaged);
#endif
}

class X
{
  int i_;

public:
  __host__ __device__ constexpr explicit X(int i)
      : i_(i)
  {}
  __host__ __device__ constexpr X(X&& x)
      : i_(x.i_)
  {
    x.i_ = 0;
  }
  __host__ __device__ TEST_CONSTEXPR_CXX20 ~X()
  {
    i_ = 0;
  }
  __host__ __device__ friend constexpr bool operator==(const X& x, const X& y)
  {
    return x.i_ == y.i_;
  }
};

class Z
{
public:
  __host__ __device__ explicit Z(int)
  {
    TEST_THROW(6);
  }
};

__host__ __device__ TEST_CONSTEXPR_CXX14 bool test()
{
  {
    optional<int> rhs;
    test<X>(cuda::std::move(rhs));
  }
  {
    optional<int> rhs(3);
    test<X>(cuda::std::move(rhs));
  }

  return true;
}

int main(int, char**)
{
#if TEST_STD_VER > 2017 && defined(_LIBCUDACXX_ADDRESSOF)
  static_assert(test());
#endif
  test();
  {
    optional<int> rhs;
    test<Z>(cuda::std::move(rhs));
  }
  {
    optional<int> rhs(3);
    test<Z>(cuda::std::move(rhs), true);
  }

  return 0;
}
