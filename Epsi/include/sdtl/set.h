/*-
 * Copyright (c) 2007-2012 Dominique Li <dominique.li@univ-tours.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: set.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_SET_H_
#define _SDTL_SET_H_

#ifndef __sdtl_set_class
#define __sdtl_set_class
#endif

#include <list>
#include <set>
#include <unordered_set>
#include <vector>
#include "types.h"

namespace sdtl {

/**
 * Set is a std::set wrapper for fast access.
 */
template<class T_>
class set : public std::unordered_set<T_> {
public:
  set() = default;
  inline explicit set(const std::list<T_> &x) {
    for (auto &&v: x) {
      this->emplace(v);
    }
  }
  inline explicit set(const std::set<T_> &x) {
    for (auto &&v: x) {
      this->emplace(v);
    }
  }
  inline explicit set(const std::vector<T_> &x) {
    for (auto &&v: x) {
      this->emplace(v);
    }
  }
};

template<class T_>
set<T_> &operator<<(set<T_> &c, const std::list<T_> &list) {
  c.clear();
  for (auto &&v: list) {
    c.emplace(v);
  }
  return c;
}

template<class T_>
set<T_> &operator<<(set<T_> &c, const std::set<T_> &x) {
  c.clear();
  for (auto &&v: x) {
    c.emplace(v);
  }
  return c;
}

template<class T_> set<T_> &operator<<(set<T_> &c, const std::vector<T_> &x) {
  c.clear();
  for (auto &&v: x) {
    c.emplace(v);
  }
  return c;
}

}

#endif /* _SDTL_SET_H_ */
