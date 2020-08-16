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
 * $Id: vector.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_VECTOR_H_
#define _SDTL_VECTOR_H_

#ifndef __sdtl_vector_class
#define __sdtl_vector_class
#endif

#include "list.h"
#include "set.h"
#include "types.h"
#include <cmath>
#include <list>
#include <set>
#include <vector>

namespace sdtl {

/**
 * Vector is a std::vector wrapper for size_t.
 */
template<class T_> class vector : public std::vector<T_> {
public:
  /**
   *
   */
  vector() = default;
  /**
   *
   */
  vector(const vector<T_> &) = default;
  /**
   *
   * @param x
   */
  explicit vector(const std::list<T_> &x) {
    for (auto &&v: x) {
      std::vector<T_>::emplace(v);
    }
  }
  /**
   *
   * @param x
   */
  explicit vector(const std::set<T_> &x) {
    for (auto &&v: x) {
      std::vector<T_>::emplace(v);
    }
  }
  /**
   *
   * @param x
   */
  explicit vector(const std::vector<T_> &x) {
    for (auto &&v: x) {
      std::vector<T_>::emplace(v);
    }
  }
  /**
   *
   */
  sdtl::size_t ID = 0;
};

template<class T_> vector<T_> &operator<<(vector<T_> &c, const std::list<T_> &x) {
  c.clear();
  for (auto &&v: x) {
    std::vector<T_>::emplace(v);
  }
  return c;
}

template<class T_> vector<T_> &operator<<(vector<T_> &c, const std::set<T_> &x) {
  c.clear();
  for (auto &&v: x) {
    std::vector<T_>::emplace(v);
  }
  return c;
}

template<class T_> vector<T_> &operator<<(vector<T_> &c, const std::vector<T_> &x) {
  c.clear();
  for (auto &&v: x) {
    std::vector<T_>::emplace(v);
  }
  return c;
}

/**
 * Product of the vector and a constant.
 */
template<class T_> vector<T_> operator*(const vector<T_> &v, const double d) {
  vector<T_> nv;
  for (auto &&x: v) {
    nv.push_back(x * d);
  }
  return nv;
}

/**
 * Distance (Euclidean) between two vectors.
 */
template<class T_> double operator-(const vector<T_> &v1, const vector<T_> &v2) {
  double d = 0.0;
  if (v1.size() != v2.size()) {
    throw;
  }
  for (size_t i = 0; i < v1.size(); ++i) {
    d += pow((v1[i] - v2[i]), 2);
  }
  return sqrt(d);
}

}

#endif /* _SDTL_VECTOR_H_ */
