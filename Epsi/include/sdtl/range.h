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
 * $Id: range.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_RANGE_H_
#define _SDTL_RANGE_H_

#ifndef __sdtl_range_class
#define __sdtl_range_class
#endif

namespace sdtl {

/**
 * A range.
 */
template<class T_>
struct range {
  T_ left;
  T_ right;
  range() = default;
  range(const T_ &, const T_ &);
  explicit range(const T_ &);
  explicit range(bool);
  inline bool operator()(const T_ &) const;
  inline bool operator[](const T_ &) const;
  inline range<T_> &operator=(const T_ &);
  inline range<T_> &operator=(bool);
  inline range<T_> &operator=(int);
  inline range<T_> &operator+(const range<T_> &);
  inline range<T_> &operator+(const T_ &);
  inline range<T_> &operator+=(const range<T_> &);
  inline range<T_> &operator+=(const T_ &);
};

/**
 *
 */
template<class T_>
range<T_>::range(const T_ &l, const T_ &r) : left(l), right(r) {
}

/**
 *
 */
template<class T_>
range<T_>::range(const T_ &v) : left(v), right(v) {
}

/**
 *
 */
template<class T_>
range<T_>::range(bool v) {
  if (v) {
    left = 2;
    right = 1;
  } else {
    left = 1;
    right = 0;
  }
}

/**
 *
 */
template<class T_>
inline bool range<T_>::operator()(const T_ &v) const {
  return v > left && v < right;
}

/**
 *
 */
template<class T_>
inline bool range<T_>::operator[](const T_ &v) const {
  return v >= left && v <= right;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator=(const T_ &v) {
  left = v;
  right = v;
  return *this;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator=(bool v) {
  if (v) {
    left = 2;
    right = 1;
  } else {
    left = 1;
    right = 0;
  }
  return *this;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator=(int) {
  left = 0;
  right = 0;
  return *this;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator+(const range<T_> &r) {
  if (r.left < left) {
    left = r.left;
  } else if (r.right > right) {
    right = r.right;
  }
  return *this;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator+(const T_ &v) {
  if (v < left) {
    left = v;
  } else if (v > right) {
    right = v;
  }
  return *this;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator+=(const range<T_> &r) {
  return *this + r;
}

/**
 *
 */
template<class T_>
inline range<T_> &range<T_>::operator+=(const T_ &v) {
  return *this + v;
}

/**
 *
 */
template<class T_>
bool operator!(const range<T_> &r) {
  return !r.left && !r.right;
}

/**
 *
 */
template<class T_>
bool operator==(const range<T_> &r1, const range<T_> &r2) {
  return r1.left == r2.left && r1.right == r2.right;
}

/**
 *
 */
template<class T_>
bool operator!=(const range<T_> &r1, const range<T_> &r2) {
  return !(r1 == r2);
}

/**
 *
 */
template<class T_>
bool operator<(const range<T_> &r1, const range<T_> &r2) {
  return r1.left > r2.left && r1.right < r2.right;
}

/**
 *
 */
template<class T_>
bool operator<=(const range<T_> &r1, const range<T_> &r2) {
  return r1.left >= r2.left && r1.right <= r2.right;
}

/**
 *
 */
template<class T_>
bool operator>(const range<T_> &r1, const range<T_> &r2) {
  return r1.left < r2.left && r1.right > r2.right;
}

/**
 *
 */
template<class T_>
bool operator>=(const range<T_> &r1, const range<T_> &r2) {
  return r1.left <= r2.left && r1.right >= r2.right;
}

/**
 *
 */
template<class T_>
bool operator==(const T_ &v, const range<T_> &r) {
  return v >= r.left && v <= r.right;
}

/**
 *
 */
template<class T_>
bool operator!=(const T_ &v, const range<T_> &r) {
  return !(v == r);
}

/**
 *
 */
template<class T_>
bool operator<(const T_ &v, const range<T_> &r) {
  return v < r.left;
}

/**
 *
 */
template<class T_>
bool operator<=(const T_ &v, const range<T_> &r) {
  return v < r.left || v == r;
}

/**
 *
 */
template<class T_>
bool operator>(const T_ &v, const range<T_> &r) {
  return v > r.right;
}

/**
 *
 */
template<class T_>
bool operator>=(const T_ &v, const range<T_> &r) {
  return v > r.right || v == r;
}

/**
 *
 */
template<class T_>
bool operator==(const range<T_> &r, const T_ &v) {
  return r.left <= v && r.right >= v;
}

/**
 *
 */
template<class T_>
bool operator==(const range<T_> &r, bool v) {
  return v ? r.left == 2 && r.right == 1 : r.left == 1 && r.right == 0;
}

/**
 *
 */
template<class T_>
bool operator!=(const range<T_> &r, const T_ &v) {
  return !(r == v);
}

/**
 *
 */
template<class T_>
bool operator<(const range<T_> &r, const T_ &v) {
  return r.right < v;
}

/**
 *
 */
template<class T_>
bool operator<=(const range<T_> &r, const T_ &v) {
  return r.right < v || r == v;
}

/**
 *
 */
template<class T_>
bool operator>(const range<T_> &r, const T_ &v) {
  return r.left > v;
}

/**
 *
 */
template<class T_>
bool operator>=(const range<T_> &r, const T_ &v) {
  return r.left > v || r == v;
}

}

#endif /* _SDTL_RANGE_H_ */
