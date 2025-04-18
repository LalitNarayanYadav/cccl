//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

// <cuda/std/iterator>

// reverse_iterator

// template <class U>
// reverse_iterator& operator=(const reverse_iterator<U>& u); // constexpr since C++17

#include <cuda/std/cassert>
#include <cuda/std/iterator>

#include "test_iterators.h"
#include "test_macros.h"

template <class It, class U>
__host__ __device__ constexpr void test(U u)
{
  const cuda::std::reverse_iterator<U> r2(u);
  cuda::std::reverse_iterator<It> r1;
  cuda::std::reverse_iterator<It>& rr = r1 = r2;
  assert(base(r1.base()) == base(u));
  assert(&rr == &r1);
}

struct Base
{};
struct Derived : Base
{};

struct ToIter
{
  typedef cuda::std::bidirectional_iterator_tag iterator_category;
  typedef char* pointer;
  typedef char& reference;
  typedef char value_type;
  typedef value_type difference_type;

  __host__ __device__ explicit constexpr ToIter()
      : m_value(0)
  {}
  __host__ __device__ constexpr ToIter(const ToIter& src)
      : m_value(src.m_value)
  {}
  // Intentionally not defined, must not be called.
  __host__ __device__ ToIter(char* src);
  __host__ __device__ constexpr ToIter& operator=(char* src)
  {
    m_value = src;
    return *this;
  }
  __host__ __device__ constexpr ToIter& operator=(const ToIter& src)
  {
    m_value = src.m_value;
    return *this;
  }
  char* m_value;

  __host__ __device__ reference operator*() const;
  __host__ __device__ ToIter& operator++();
  __host__ __device__ ToIter& operator--();
  __host__ __device__ ToIter operator++(int);
  __host__ __device__ ToIter operator--(int);
};

__host__ __device__ constexpr bool tests()
{
  Derived d{};
  test<bidirectional_iterator<Base*>>(bidirectional_iterator<Derived*>(&d));
  test<random_access_iterator<const Base*>>(random_access_iterator<Derived*>(&d));
  test<Base*>(&d);

  char c   = '\0';
  char* fi = &c;
  const cuda::std::reverse_iterator<char*> rev_fi(fi);
  cuda::std::reverse_iterator<ToIter> rev_ti;
  rev_ti = rev_fi;
  assert(rev_ti.base().m_value == fi);

  return true;
}

int main(int, char**)
{
  tests();
  static_assert(tests(), "");
  return 0;
}
