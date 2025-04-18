//===----------------------------------------------------------------------===//
//
// Part of libcu++, the C++ Standard Library for your entire system,
// under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#include "test_macros.h"

TEST_DIAG_SUPPRESS_MSVC(4505)

#include <cuda/annotated_ptr>
#include <cuda/std/cassert>

#include <nv/target>

#if defined(DEBUG)
#  define DPRINTF(...)     \
    {                      \
      printf(__VA_ARGS__); \
    }
#else
#  define DPRINTF(...) \
    do                 \
    {                  \
    } while (false)
#endif

__device__ __host__ void assert_rt_wrap(cudaError_t code, const char* file, int line)
{
  if (code != cudaSuccess)
  {
#if !TEST_COMPILER(NVRTC)
    NV_IF_ELSE_TARGET(NV_IS_HOST,
                      (printf("assert: %s %s %d\n", cudaGetErrorString(code), file, line);),
                      (printf("assert: error=%d %s %d\n", code, file, line);))
#endif // !TEST_COMPILER(NVRTC)
    assert(code == cudaSuccess);
  }
}
#define assert_rt(ret)                         \
  {                                            \
    assert_rt_wrap((ret), __FILE__, __LINE__); \
  }

template <typename T, int N>
__device__ __host__ __noinline__ T* global_alloc()
{
  T* arr = nullptr;

  NV_IF_ELSE_TARGET(
    NV_IS_DEVICE, (arr = (T*) malloc(N * sizeof(T));), (assert_rt(cudaMallocManaged((void**) &arr, N * sizeof(T)));))

  for (int i = 0; i < N; ++i)
  {
    arr[i] = i;
  }
  return arr;
}

template <typename T, int N>
__device__ __noinline__ T* shared_alloc()
{
  __shared__ T data[N];

  for (int i = 0; i < N; ++i)
  {
    data[i] = i;
  }
  return data;
}

template <typename T>
__device__ __host__ __noinline__ void dealloc(T* arr)
{
  NV_IF_ELSE_TARGET(NV_IS_DEVICE, (free(arr);), assert_rt(cudaFree(arr));)
}
