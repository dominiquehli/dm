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
 * $Id: set.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_RELATION_SET_H_
#define _SDTL_RELATION_SET_H_

namespace sdtl {

// ---

template<class _ITEM> bool eq(const set<_ITEM> &, const set<_ITEM> &);

// ---

template<class _ITEM> bool neq(const set<_ITEM> &, const set<_ITEM> &);

// ---

template<class _ITEM> bool subset(const set<_ITEM> &, const set<_ITEM> &);

// ---

template<class _ITEM> bool subseteq(const set<_ITEM> &, const set<_ITEM> &);

// ---

template<class _ITEM> bool operator==(const set<_ITEM> &, const set<_ITEM> &);

template<class _ITEM> bool operator!=(const set<_ITEM> &, const set<_ITEM> &);

template<class _ITEM> bool operator<(const set<_ITEM> &, const set<_ITEM> &);

template<class _ITEM> bool operator<=(const set<_ITEM> &, const set<_ITEM> &);

template<class _ITEM> bool operator>(const set<_ITEM> &, const set<_ITEM> &);

template<class _ITEM> bool operator>=(const set<_ITEM> &, const set<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Relations on sets.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
template<class _ITEM> bool eq(const set<_ITEM> &x, const set<_ITEM> &y) {
  if (!x.size()) {
    return !y.size();
  } // If x is empty, then y must be empty.
  size_t xi = 0;
  size_t yi = 0;
  while (xi < x.size()) {
    if (yi == y.size()) {
      return false;
    }
    if (x[xi] != y[yi]) {
      return false;
    }
    ++xi;
    ++yi;
  }
  return yi == y.size();
}

/**
 *
 */
template<class _ITEM> bool neq(const set<_ITEM> &x, const set<_ITEM> &y) {
  return !eq(x, y);
}

/**
 *
 */
template<class _ITEM> bool subset(const set<_ITEM> &x, const set<_ITEM> &y) {
  if (x.size() >= y.size()) {
    return false;
  }
  return subseteq(x, y);
}

/**
 *
 */
template<class _ITEM> bool subseteq(const set<_ITEM> &x, const set<_ITEM> &y) {
  if (!x.size()) {
    return true;
  }
  if (x.size() > y.size()) {
    return false;
  }
  size_t xi = 0;
  size_t yi = 0;
  while (xi < x.size()) {
    if (yi == y.size()) {
      return false;
    }
    if (x[xi] == y[yi]) {
      ++xi;
    }
    ++yi;
  }
  return true;
}

/**
 * Test equivalence of two sets.
 */
template<class _ITEM> bool operator==(const set<_ITEM> &x, const set<_ITEM> &y) {
  if (x.size() != y.size()) {
    return false;
  }
  for (size_t xi = 0, yi = 0; xi < x.size(); ++xi, ++yi) {
    if (x[xi] != y[yi]) {
      return false;
    }
  }
  return true;
}

/**
 * Test difference of two sets.
 */
template<class _ITEM> bool operator!=(const set<_ITEM> &x, const set<_ITEM> &y) {
  return !(x == y);
}

/**
 * Test subset relation of two sets.
 */
template<class _ITEM> bool operator<(const set<_ITEM> &x, const set<_ITEM> &y) {
  return x.size() < y.size() && subseteq(x, y);
}

/**
 * Test subset-equivalence relation of two sets.
 */
template<class _ITEM> bool operator<=(const set<_ITEM> &x, const set<_ITEM> &y) {
  return subseteq(x, y);
}

/**
 * Test superset relation of two sets.
 */
template<class _ITEM> bool operator>(const set<_ITEM> &x, const set<_ITEM> &y) {
  return subset(y, x);
}

/**
 * Test superset-equivalence relation of two sets.
 */
template<class _ITEM> bool operator>=(const set<_ITEM> &x, const set<_ITEM> &y) {
  return subseteq(y, x);
}

}

#endif /* _SDTL_RELATION_SET_H_ */
