
#ifndef __EMBREE_PLATFORM_H__
#define __EMBREE_PLATFORM_H__

#include <stddef.h>
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <stdexcept>

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>

#if defined(__x86_64__) || defined(__ia64__) || defined(_M_X64)
#define __X86_64__
#endif

#if defined(linux) || defined(__linux__) || defined(__LINUX__)
#  if !defined(__LINUX__)
#     define __LINUX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

#if defined(__FreeBSD__) || defined(__FREEBSD__)
#  if !defined(__FREEBSD__)
#     define __FREEBSD__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)) && !defined(__CYGWIN__)
#  if !defined(__WIN32__)
#     define __WIN32__
#  endif
#endif

#if defined(__CYGWIN__)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

#if defined(__APPLE__) || defined(MACOSX) || defined(__MACOSX__)
#  if !defined(__MACOSX__)
#     define __MACOSX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

#if defined(__unix__) || defined (unix) || defined(__unix) || defined(_unix)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

#if defined(_MSC_VER) && !defined(__SSE__)
#define __SSE__
#endif

#if defined(_MSC_VER) && !defined(__SSE4_2__) && !defined(__SSE4_1__)
#define __SSE4_1__
#define __SSE4_2__
#endif

#if defined(_MSC_VER) && !defined(__AVX__)

#endif

#ifdef __WIN32__
#define __dllexport extern "C" __declspec(dllexport)
#define __dllimport extern "C" __declspec(dllimport)
#else
#define __dllexport extern "C" __attribute__ ((visibility ("default")))
#define __dllimport extern "C"
#endif

#ifdef __WIN32__
#undef __noinline
#define __noinline             __declspec(noinline)

#define __thread               __declspec(thread)
#define __align(...)           __declspec(align(__VA_ARGS__))

#define debugbreak()           __debugbreak()

#else
#undef __noinline
#undef __forceinline
#define __noinline             __attribute__((noinline))
#define __forceinline          inline __attribute__((always_inline))

#define __align(...)           __attribute__((aligned(__VA_ARGS__)))
#define __FUNCTION__           __PRETTY_FUNCTION__
#define debugbreak()           asm ("int $3")
#endif

#ifdef __GNUC__
#define MAYBE_UNUSED __attribute__((used))
#else
#define MAYBE_UNUSED
#endif

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define   likely(expr) expr
#define unlikely(expr) expr
#else
#define   likely(expr) __builtin_expect(expr,true )
#define unlikely(expr) __builtin_expect(expr,false)
#endif

#ifdef __GNUC__
#  define __memory_barrier() asm volatile("" ::: "memory")
#elif defined(__MIC__)
#define __memory_barrier()
#elif defined(__INTEL_COMPILER)

#elif  defined(_MSC_VER)
#  define __memory_barrier() _ReadWriteBarrier()
#endif

#define STRING(x) #x
#define PING std::cout << __FILE__ << " (" << __LINE__ << "): " << __FUNCTION__ << std::endl
#define PRINT(x) std::cout << STRING(x) << " = " << (x) << std::endl
#define PRINT2(x,y) std::cout << STRING(x) << " = " << (x) << ", " << STRING(y) << " = " << (y) << std::endl
#define PRINT3(x,y,z) std::cout << STRING(x) << " = " << (x) << ", " << STRING(y) << " = " << (y) << ", " << STRING(z) << " = " << (z) << std::endl
#define PRINT4(x,y,z,w) std::cout << STRING(x) << " = " << (x) << ", " << STRING(y) << " = " << (y) << ", " << STRING(z) << " = " << (z) << ", " << STRING(w) << " = " << (w) << std::endl

#define FORCE_LINK_THIS(unit) \
  bool unit##_force_link_me = true;

#define FORCE_LINK_THAT(unit) \
  extern bool unit##_force_link_me; bool unit##_force_link = unit##_force_link_me;

typedef          long long  int64;
typedef unsigned long long uint64;
typedef                int  int32;
typedef unsigned       int uint32;
typedef              short  int16;
typedef unsigned     short uint16;
typedef               char   int8;
typedef unsigned      char  uint8;

#ifdef __WIN32__
#if defined(__X86_64__)
typedef int64 ssize_t;
#else
typedef int32 ssize_t;
#endif
#endif

#if defined(__INTEL_COMPILER)
#pragma warning(disable:265 )
#pragma warning(disable:383 )
#pragma warning(disable:869 )
#pragma warning(disable:981 )
#pragma warning(disable:1418)
#pragma warning(disable:1419)
#pragma warning(disable:1572)
#pragma warning(disable:94  )
#pragma warning(disable:1599)
#pragma warning(disable:424 )
#endif

#if defined(_MSC_VER)
#pragma warning(disable:4200)
#pragma warning(disable:4800)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4355)
#pragma warning(disable:4996)
#endif

#include "constants.h"

namespace embree
{
#define ALIGNED_CLASS                                                \
  public:                                                            \
    void* operator new(size_t size) { return alignedMalloc(size); }  \
  void operator delete(void* ptr) { alignedFree(ptr); }      \
  void* operator new[](size_t size) { return alignedMalloc(size); }  \
  void operator delete[](void* ptr) { alignedFree(ptr); }    \
 private:

#define ALIGNED_CLASS_(align)                                           \
  public:                                                               \
    void* operator new(size_t size) { return alignedMalloc(size,align); } \
  void operator delete(void* ptr) { alignedFree(ptr); }                 \
  void* operator new[](size_t size) { return alignedMalloc(size,align); } \
  void operator delete[](void* ptr) { alignedFree(ptr); }               \
 private:

    void* alignedMalloc(size_t size, size_t align = 64);
    void alignedFree(const void* ptr);

    void* os_malloc(size_t bytes);
    void* os_reserve(size_t bytes);
    void  os_commit(void* ptr, size_t bytes);
    void  os_free(void* ptr, size_t bytes);

    double getSeconds();
}

#endif