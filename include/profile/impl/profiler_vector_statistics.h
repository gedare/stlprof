// -*- C++ -*-
//
// Copyright (C) 2009, 2010 Free Software Foundation, Inc.
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

/** @file profile/impl/profiler_vector_statistics.h
 *  @brief Collection of vector statistics traces.
 */

// Written by Lixia Liu and Silvius Rus.

#ifndef _GLIBCXX_PROFILE_PROFILER_VECTOR_STATISTICS_H
#define _GLIBCXX_PROFILE_PROFILER_VECTOR_STATISTICS_H 1

#include "profile/impl/profiler.h"
#include "profile/impl/profiler_node.h"
#include "profile/impl/profiler_trace.h"
#include "profile/impl/profiler_state.h"
#include "profile/impl/profiler_container_statistics.h"

namespace __gnu_profile
{
  /** @brief vector statistics instrumentation trace producer.  */
  class __trace_vector_statistics
    : public __trace_container_statistics
  {
    public:
      __trace_vector_statistics()
        : __trace_container_statistics()
      { __id = "vector-statistics"; }
  };

  inline void
    __trace_vector_statistics_init()
    { _GLIBCXX_PROFILE_DATA(_S_vector_statistics) = new __trace_vector_statistics(); }

  inline void
    __trace_vector_statistics_report(FILE* __f, __warning_vector_t& __warnings)
    {
      if (_GLIBCXX_PROFILE_DATA(_S_vector_statistics))
      {
        _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__collect_warnings(__warnings);
        _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__write(__f);
      }
    }

  inline void
    __trace_vector_statistics_construct(const void* __obj, std::size_t __num)
    {
      if (!__profcxx_init())
        return;

      _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__insert(__obj, __get_stack(),
          __num);
    }

  inline void
    __trace_vector_statistics_destruct(const void* __obj, std::size_t __num,
        std::size_t __inum)
    {
      if (!__profcxx_init())
        return;

      _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_destruct(__obj, __num, __inum);
    }

  inline void
  __trace_vector_statistics_insert_pre(const void* __obj, std::size_t __pos,
				std::size_t __num)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_insert_pre(__obj, __pos, __num);
  }

  inline void
  __trace_vector_statistics_insert_post(const void* __obj, std::size_t __pos,
				std::size_t __num)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_insert_post(__obj, __pos, __num);
  }

  inline void
  __trace_vector_statistics_erase_pre(const void* __obj, std::size_t __pos,
				std::size_t __num)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_erase_pre(__obj, __pos, __num);
  }

  inline void
  __trace_vector_statistics_erase_post(const void* __obj, std::size_t __pos,
				std::size_t __num)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_erase_post(__obj, __pos, __num);
  }

  inline void
  __trace_vector_statistics_invalid_operator(const void* __obj)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_invalid_operator(__obj);
  }

  inline void
    __trace_vector_statistics_resize(const void* __obj, std::size_t __from,
        std::size_t __to)
    {
      if (!__profcxx_init())
        return;

      _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_resize(__obj, __from, __to);
    }

  inline void
  __trace_vector_statistics_push_back_pre(const void* __obj)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_push_back_pre(__obj);
  }

  inline void
  __trace_vector_statistics_push_back_post(const void* __obj)
  {
    if (!__profcxx_init())
      return;

    _GLIBCXX_PROFILE_DATA(_S_vector_statistics)->__opr_push_back_post(__obj);
  }


} // namespace __gnu_profile

#endif /* _GLIBCXX_PROFILE_PROFILER_VECTOR_STATISTICS_H */
