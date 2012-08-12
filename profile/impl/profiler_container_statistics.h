// -*- C++ -*-
//
// Copyright (C) 2009, 2010, 2011 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

/** @file profile/impl/profiler_container_statistics.h
 *  @brief Diagnostics for container statisticss.
 */

// Written by Lixia Liu and Silvius Rus.

#ifndef _GLIBCXX_PROFILE_PROFILER_CONTAINER_STATISTICS_H
#define _GLIBCXX_PROFILE_PROFILER_CONTAINER_STATISTICS_H 1

#include <sstream>

#include "profile/impl/profiler.h"
#include "profile/impl/profiler_node.h"
#include "profile/impl/profiler_trace.h"

namespace __gnu_profile
{

  struct __timing_data
  {
    unsigned long long __time;
    unsigned long long __latency;

    __timing_data() : __time(0), __latency(0) { }
    __timing_data(unsigned long long __t, unsigned long long __l)
      : __time(__t), __latency(__l) { }
    bool operator<(const __timing_data& __other) const
    { return __latency < __other.__latency; }
  };
  typedef std::_GLIBCXX_STD_C::vector<__timing_data> __timing_vector_t;

#define __write_timing_vector_t(NAME)                                     \
  for ( typename __timing_vector_t::const_iterator __it                   \
      = NAME.begin(); __it != NAME.end(); ++__it ) {                  \
    std::fprintf(__f, " %llu %llu", (*__it).__time, (*__it).__latency);  \
  }

  /** @brief A container statistics instrumentation line in the object table. */
  class __container_statistics_info
    : public __object_info_base 
  {
    public:
      __container_statistics_info()
        : _M_init(0), _M_max(0), _M_min(0), _M_total(0), _M_item_min(0),
        _M_item_max(0), _M_item_total(0), _M_count(0), _M_resize(0), _M_cost(0),
        _M_insert(0), _M_find(0), _M_push_back(0),
        _M_insert_time(0),
        _M_find_time(0),
        _M_push_back_time(0)
    { }

      __container_statistics_info(const __container_statistics_info& __o)
        : __object_info_base(__o), _M_init(__o._M_init), _M_max(__o._M_max),
        _M_min(__o._M_min), _M_total(__o._M_total),
        _M_item_min(__o._M_item_min), _M_item_max(__o._M_item_max),
        _M_item_total(__o._M_item_total), _M_count(__o._M_count),
        _M_resize(__o._M_resize), _M_cost(__o._M_cost),
        _M_insert(__o._M_insert),
        _M_find(__o._M_find), _M_push_back(__o._M_push_back),
        _M_insert_time(__o._M_insert_time), __insert_time(__o.__insert_time),
        _M_find_time(__o._M_find_time), __find_time(__o.__find_time),
        _M_push_back_time(__o._M_push_back_time),
        __push_back_time(__o.__push_back_time)
    { }

      __container_statistics_info(__stack_t __stack, std::size_t __num)
        : __object_info_base(__stack), _M_init(__num), _M_max(__num),
        _M_min(0), _M_total(0), _M_item_min(0), _M_item_max(0),
        _M_item_total(0), _M_count(0), _M_resize(0), _M_cost(0),
        _M_insert(0), _M_find(0), _M_push_back(0),
        _M_insert_time(0),
        _M_find_time(0),
        _M_push_back_time(0)
    { }

      virtual ~__container_statistics_info() { }

      void
        __write(FILE* __f) const
        {
          std::fprintf(
              __f,
              "%Zu %Zu %Zu %Zu %Zu %Zu %Zu %Zu %Zu %Zu %Zu %Zu %Zu", 
              _M_init, _M_count, _M_cost, _M_resize, _M_min, _M_max,
              _M_total, _M_item_min, _M_item_max, _M_item_total,
              _M_insert, _M_find, _M_push_back
          );
          std::fprintf(
              __f,
              " %llu %llu %llu",
              _M_insert_time,
              _M_find_time,
              _M_push_back_time
          );
          std::fprintf(__f, "\ninsert" );
          __write_timing_vector_t(__insert_time);
          std::fprintf(__f, "\nfind" );
          __write_timing_vector_t(__find_time);
          std::fprintf(__f, "\npush_back" );
          __write_timing_vector_t(__push_back_time);
          std::fprintf(__f, "\n" );
        }

      float
        __magnitude() const
        { return static_cast<float>(_M_cost); }

      std::string
        __advice() const
        {
          std::stringstream __message;
          if (_M_init < _M_item_max)
            __message << "change initial container statistics from " << _M_init
              << " to " << _M_item_max;
          return __message.str();
        }

      void
        __merge(const __container_statistics_info& __o)
        {
          _M_init        = std::max(_M_init, __o._M_init);
          _M_max         = std::max(_M_max, __o._M_max);
          _M_item_max    = std::max(_M_item_max, __o._M_item_max);
          _M_min         = std::min(_M_min, __o._M_min);
          _M_item_min    = std::min(_M_item_min, __o._M_item_min);
          _M_total      += __o._M_total;
          _M_item_total += __o._M_item_total;
          _M_count      += __o._M_count;
          _M_cost       += __o._M_cost;
          _M_insert     += __o._M_insert;
          _M_find       += __o._M_find;
          _M_push_back  += __o._M_push_back;
          _M_resize     += __o._M_resize;
          _M_insert_time += __o._M_insert_time;
          _M_find_time += __o._M_find_time;
          _M_push_back_time += __o._M_push_back_time;
        }

      // Call if a container is destructed or cleaned.
      void
        __opr_destruct(std::size_t __num, std::size_t __inum)
        {
          _M_max = std::max(_M_max, __num);
          _M_item_max = std::max(_M_item_max, __inum);
          if (_M_min == 0)
          {
            _M_min = __num; 
            _M_item_min = __inum;
          }
          else
          {
            _M_min = std::min(_M_min, __num);
            _M_item_min = std::min(_M_item_min, __inum);
          }
          _M_total += __num;
          _M_item_total += __inum;
          _M_count += 1;
        }

      void
        __opr_insert(std::size_t __pos, std::size_t __num)
        { _M_insert += 1; } // FIXME: add __num? have to hook from insert_post

      void
        __opr_insert_pre(std::size_t __pos, std::size_t __num)
        {
          __opr_insert(__pos, __num);
          t1 = __builtin_ia32_rdtsc();
        }

      void
        __opr_insert_post(unsigned long long t2)
        {
          _M_insert_time += (t2 - t1);
          __insert_time.push_back(__timing_data(t2, _M_insert_time));
        }

      void
        __opr_invalid_operator(const void* __obj)
        {
//          _M_valid = false;
        }

      void
        __opr_find(const void* __obj, std::size_t __size)
        { _M_find += 1; }

      void
        __opr_find_pre(const void* __obj, std::size_t __size)
        {
          __opr_find(__obj, __size);
          t1 = __builtin_ia32_rdtsc();
        }

      void
        __opr_find_post(unsigned long long t2)
        {
          _M_find_time += (t2 - t1);
          __find_time.push_back(__timing_data(t2, _M_find_time));
        }

      // list objects
      // Estimate the cost of resize/rehash. 
      float
        __resize_cost(std::size_t __from, std::size_t)
        { return __from; }

      void
        __opr_resize(std::size_t __from, std::size_t __to)
        {
//          _M_cost += this->__resize_cost(__from, __to);
          _M_resize += 1;
          _M_max = std::max(_M_max, __to);
        }

      void
        __opr_push_back( )
        {_M_push_back += 1; }

      void
        __opr_push_back_pre( )
        {
          __opr_push_back();
          // FIXME: non-portable check std::chrono::high_resolution_clock::now()
          t1 = __builtin_ia32_rdtsc();
        }

      void
        __opr_push_back_post(unsigned long long t2)
        {
          _M_push_back_time += (t2 - t1);
          __push_back_time.push_back(__timing_data(t1, _M_push_back_time));
        }

    private:
      std::size_t _M_init;
      std::size_t _M_max;  // range of # buckets
      std::size_t _M_min;
      std::size_t _M_total;
      std::size_t _M_item_min;  // range of # items
      std::size_t _M_item_max;
      std::size_t _M_item_total;
      std::size_t _M_count;
      std::size_t _M_insert;
      std::size_t _M_find;
      std::size_t _M_push_back;
      std::size_t _M_resize;
      std::size_t _M_cost;

      unsigned long long t1;

      unsigned long long _M_insert_time; 
      __timing_vector_t __insert_time;

      unsigned long long _M_find_time;
      __timing_vector_t __find_time;

      unsigned long long _M_push_back_time;
      __timing_vector_t __push_back_time;
  };


  /** @brief A container statistics instrumentation line in the stack table.  */
  class __container_statistics_stack_info
    : public __container_statistics_info
  {
    public:
      __container_statistics_stack_info(const __container_statistics_info& __o)
        : __container_statistics_info(__o) { }
  };


  /** @brief Container size instrumentation trace producer.  */
  class __trace_container_statistics
    : public __trace_base<__container_statistics_info, __container_statistics_stack_info> 
  {
    public:
      ~__trace_container_statistics() { }

      __trace_container_statistics()
        : __trace_base<__container_statistics_info, __container_statistics_stack_info>() { };

      // Insert a new node at construct with object, callstack and initial size. 
      void
        __insert(const __object_t __obj, __stack_t __stack, std::size_t __num)
        { __add_object(__obj, __container_statistics_info(__stack, __num)); }

      // XXX Undefined?
      void
        __construct(const void* __obj, std::size_t __inum);

      // Call at destruction/clean to set container final size.
      void
        __opr_destruct(const void* __obj, std::size_t __num, std::size_t __inum)
        {
          if (!__is_on())
            return;

          __object_t __obj_handle = static_cast<__object_t>(__obj);

          __container_statistics_info* __object_info = __get_object_info(__obj_handle);
          if (!__object_info)
            return;

          __object_info->__opr_destruct(__num, __inum);
          __retire_object(__obj_handle);
        }

      void
        __opr_insert_pre(const void* __obj, std::size_t __pos, std::size_t __num)
        {
          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_insert_pre(__pos, __num);
        }

      void
        __opr_insert_post(const void* __obj, std::size_t __pos, std::size_t __num)
        {
          unsigned long long t2 = __builtin_ia32_rdtsc(); // FIXME: non-portable
          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_insert_post(t2);
        }

      void
        __opr_invalid_operator(const void* __obj)
        {
          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_invalid_operator(__obj);
        }


      // Call at resize to set resize/cost information.
      void
        __opr_resize(const void* __obj, int __from, int __to)
        {
          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_resize(__from, __to);
        }

      void
        __opr_find_pre(const void* __obj, std::size_t __size)
        {
          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_find_pre(__obj, __size);
        }

      void
        __opr_find_post(const void* __obj, std::size_t __size)
        {
          unsigned long long t2 = __builtin_ia32_rdtsc(); // FIXME: non-portable

          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_find_post(t2);
        }

      void
        __opr_push_back_pre(const void* __obj)
        {
          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_push_back_pre();
        }

      void
        __opr_push_back_post(const void* __obj)
        {
          unsigned long long t2 = __builtin_ia32_rdtsc(); // FIXME: non-portable

          if (!__is_on())
            return;

          __container_statistics_info* __object_info = __get_object_info(__obj);
          if (!__object_info)
            return;

          __object_info->__opr_push_back_post(t2);
        }

  };

} // namespace __gnu_profile
#endif /* _GLIBCXX_PROFILE_PROFILER_CONTAINER_STATISTICS_H */
