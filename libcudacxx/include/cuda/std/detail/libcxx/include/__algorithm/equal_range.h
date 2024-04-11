//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___ALGORITHM_EQUAL_RANGE_H
#define _LIBCUDACXX___ALGORITHM_EQUAL_RANGE_H

#include <cuda/std/detail/__config>

#if defined(_CCCL_IMPLICIT_SYSTEM_HEADER_GCC)
#  pragma GCC system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_CLANG)
#  pragma clang system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_MSVC)
#  pragma system_header
#endif // no system header

#include <cuda/std/detail/libcxx/include/__algorithm/comp.h>
#include <cuda/std/detail/libcxx/include/__algorithm/comp_ref_type.h>
#include <cuda/std/detail/libcxx/include/__algorithm/half_positive.h>
#include <cuda/std/detail/libcxx/include/__algorithm/iterator_operations.h>
#include <cuda/std/detail/libcxx/include/__algorithm/lower_bound.h>
#include <cuda/std/detail/libcxx/include/__algorithm/upper_bound.h>
#include <cuda/std/detail/libcxx/include/__functional/identity.h>
#include <cuda/std/detail/libcxx/include/__functional/invoke.h>
#include <cuda/std/detail/libcxx/include/__iterator/advance.h>
#include <cuda/std/detail/libcxx/include/__iterator/distance.h>
#include <cuda/std/detail/libcxx/include/__iterator/iterator_traits.h>
#include <cuda/std/detail/libcxx/include/__iterator/next.h>
#include <cuda/std/detail/libcxx/include/__type_traits/is_callable.h>
#include <cuda/std/detail/libcxx/include/__type_traits/is_copy_constructible.h>
#include <cuda/std/detail/libcxx/include/__utility/move.h>
#include <cuda/std/detail/libcxx/include/__utility/pair.h>

_LIBCUDACXX_BEGIN_NAMESPACE_STD

template <class _AlgPolicy, class _Compare, class _Iter, class _Sent, class _Tp, class _Proj>
_LIBCUDACXX_INLINE_VISIBILITY _CCCL_CONSTEXPR_CXX14 pair<_Iter, _Iter>
__equal_range(_Iter __first, _Sent __last, const _Tp& __value, _Compare&& __comp, _Proj&& __proj)
{
  auto __len  = _IterOps<_AlgPolicy>::distance(__first, __last);
  _Iter __end = _IterOps<_AlgPolicy>::next(__first, __last);
  while (__len != 0)
  {
    auto __half_len = _CUDA_VSTD::__half_positive(__len);
    _Iter __mid     = _IterOps<_AlgPolicy>::next(__first, __half_len);
    if (_CUDA_VSTD::__invoke(__comp, _CUDA_VSTD::__invoke(__proj, *__mid), __value))
    {
      __first = ++__mid;
      __len -= __half_len + 1;
    }
    else if (_CUDA_VSTD::__invoke(__comp, __value, _CUDA_VSTD::__invoke(__proj, *__mid)))
    {
      __end = __mid;
      __len = __half_len;
    }
    else
    {
      _Iter __mp1 = __mid;
      return pair<_Iter, _Iter>(_CUDA_VSTD::__lower_bound<_AlgPolicy>(__first, __mid, __value, __comp, __proj),
                                _CUDA_VSTD::__upper_bound<_AlgPolicy>(++__mp1, __end, __value, __comp, __proj));
    }
  }
  return pair<_Iter, _Iter>(__first, __first);
}

template <class _ForwardIterator, class _Tp, class _Compare>
_LIBCUDACXX_NODISCARD_EXT _LIBCUDACXX_INLINE_VISIBILITY _CCCL_CONSTEXPR_CXX14 pair<_ForwardIterator, _ForwardIterator>
equal_range(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, _Compare __comp)
{
  static_assert(__is_callable<_Compare, decltype(*__first), const _Tp&>::value, "The comparator has to be callable");
  static_assert(is_copy_constructible<_ForwardIterator>::value, "Iterator has to be copy constructible");
  return _CUDA_VSTD::__equal_range<_ClassicAlgPolicy>(
    _CUDA_VSTD::move(__first),
    _CUDA_VSTD::move(__last),
    __value,
    static_cast<__comp_ref_type<_Compare>>(__comp),
    _CUDA_VSTD::__identity());
}

template <class _ForwardIterator, class _Tp>
_LIBCUDACXX_NODISCARD_EXT _LIBCUDACXX_INLINE_VISIBILITY _CCCL_CONSTEXPR_CXX14 pair<_ForwardIterator, _ForwardIterator>
equal_range(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value)
{
  return _CUDA_VSTD::equal_range(_CUDA_VSTD::move(__first), _CUDA_VSTD::move(__last), __value, __less{});
}

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___ALGORITHM_EQUAL_RANGE_H
