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
 * $Id: sequence.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_RELATION_SEQUENCE_H_
#define _SDTL_RELATION_SEQUENCE_H_

namespace sdtl {

// ---

template<class _ITEM> range_type subseq(const sequence<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &, const sequence<_ITEM> &);

// ---

template<class _ITEM> range_type subseq(const sequence<_ITEM> &, const sequence<_ITEM> &, const range_type &);

template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &, const sequence<_ITEM> &, const range_type &);

// ---

template<class _ITEM> bool operator==(const sequence<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> bool operator!=(const sequence<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> bool operator<(const sequence<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> bool operator<=(const sequence<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> bool operator>(const sequence<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> bool operator>=(const sequence<_ITEM> &, const sequence<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Relations on sequences.
//
////////////////////////////////////////////////////////////////////////////////

/**
 * subseq 1
 */
template<class _ITEM> range_type subseq(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  return subseq(x, y, range_type(0, y.size() - x.size()));
}

/**
 * subseqeq 1
 */
template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  return subseqeq(x, y, range_type(0, y.size() - x.size()));
}

/**
 * subseq 2
 */
template<class _ITEM> range_type subseq(const sequence<_ITEM> &x, const sequence<_ITEM> &y, const range_type &r) {
  if (!x.length()) {
    return y.length();
  } // An empty sequence is subseq to non-empty sequence.
  if (x.length() > y.length()) {
    return range_type(false);
  } // If x is not empty, then y must be larger than x.
  if (x.size() == y.size())
    return range_type(0, 0);
  else
    return subseqeq(x, y, r);
}

/**
 * subseqeq 2
 */
template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &x, const sequence<_ITEM> &y, const range_type &r) {
  if (!x.length()) {
    return range_type(true);
  } // An empty sequence is subseqeq to any sequence.
  if (x.length() > y.length() || x.size() > y.size() - r.left) {
    return range_type(false);
  } // If x is not empty, then y must be larger than x.
  size_t xf = 0;
  size_t xi = 0;
  size_t yi = r.left;
  while (xi < x.size()) {
    if (yi == y.size())
      return range_type(false);
    if (x[xi] <= y[yi])
      ++xi;
    if (!xf) {
      if (!r[xi - 1])
        return range_type(false);
      xf = xi;
    }
    ++yi;
  }
  return range_type(xf - 1, yi - 1);
}

/**
 * Test equivalence of two sequences.
 */
template<class _ITEM> bool operator==(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  if (!x.length() && !y.length()) {
    return true;
  }
  if (x.length() != y.length() || x.size() != y.size()) {
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
 * Test difference of two sequences.
 */
template<class _ITEM> bool operator!=(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  return !(x == y);
}

/**
 * Test subsequence relation of two sequences.
 */
template<class _ITEM> bool operator<(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  return x.length() < y.length() && x <= y;
}

/**
 * Test subsequence relation of two sequences.
 */
template<class _ITEM> bool operator<=(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  if (!x.length()) {
    return true;
  } // An empty sequence is subseqeq to any sequence.
  if (x.length() > y.length() || x.size() > y.size()) {
    return false;
  } // If x is not empty, then y must be larger than x.
  size_t xi = 0;
  size_t yi = 0;
  while (xi < x.size()) {
    if (yi == y.size())
      return false;
    if (x[xi] <= y[yi])
      ++xi;
    ++yi;
  }
  return true;
}

/**
 * Test super-sequence relation of two sequences.
 */
template<class _ITEM> bool operator>(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  return x.length() > y.length() && x >= y;
}

/**
 * Test super-sequence relation of two sequences.
 */
template<class _ITEM> bool operator>=(const sequence<_ITEM> &x, const sequence<_ITEM> &y) {
  return y <= x;
}

}

#endif /* _SDTL_RELATION_SEQUENCE_H_ */
