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
 * $Id: attribute.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_ATTRIBUTE_H_
#define _SDTL_ATTRIBUTE_H_

#ifndef __sdtl_attribute_class
#define __sdtl_attribute_class
#endif

namespace sdtl {

/**
 * Attribute for relational data.
 */
template<class N_, class V_>
struct attribute {
  static attribute<N_, V_> zero();
  attribute() = default;
  attribute(const N_ &, const V_ &);
  virtual ~attribute() = default;
  inline attribute<N_, V_> &operator=(int);
  explicit inline operator bool() const;
  N_ name;
  V_ value;
};

/**
 *
 */
template<class N_, class V_>
attribute<N_, V_> attribute<N_, V_>::zero() {
  return attribute<N_, V_>(0, 0);
}

/**
 *
 */
template<class N_, class V_>
attribute<N_, V_>::attribute(const N_ &n, const V_ &v) : name(n), value(v) {
}

/**
 *
 */
template<class N_, class V_>
inline attribute<N_, V_> &attribute<N_, V_>::operator=(int n) {
  name = n;
  value = n;
  return *this;
}

/**
 *
 */
template<class N_, class V_>
inline attribute<N_, V_>::operator bool() const {
  return !(*this == 0);
}

/**
 * Operator == for two attributes.
 */
template<class N_, class V_>
bool operator==(const attribute<N_, V_> &a1, const attribute<N_, V_> &a2) {
  return a1.name == a2.name && a1.value == a2.value;
}

/**
 * Operator == for an attribute and an integer.
 */
template<class N_, class V_>
bool operator==(const attribute<N_, V_> &a, int n) {
  return a.value == n;
}

/**
 * Operator ! for an attribute.
 */
template<class N_, class V_>
bool operator!(const attribute<N_, V_> &a) {
  return a == 0;
}

/**
 * Operator != for two attributes.
 */
template<class N_, class V_>
bool operator!=(const attribute<N_, V_> &a1, const attribute<N_, V_> &a2) {
  return a1.name != a2.name || a1.value != a2.value;
}

/**
 * Operator != for an attribute and the zero.
 */
template<class N_, class V_>
bool operator!=(const attribute<N_, V_> &a, int n) {
  return !(a == n);
}

/**
 * Operator < for two attributes.
 */
template<class N_, class V_>
bool operator<(const attribute<N_, V_> &a1, const attribute<N_, V_> &a2) {
  return a1.value < a2.value;
}

}

#endif /* _SDTL_ATTRIBUTE_H_ */
