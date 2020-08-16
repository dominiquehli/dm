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
 * $Id: sdb.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_RELATION_SDB_H_
#define _SDTL_RELATION_SDB_H_

namespace sdtl {

// ---

template<class _ITEM> range_type subseq(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> range_type subseq(const sdbi<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> range_type subseq(const sequence<_ITEM> &, const sdbi<_ITEM> &);

// ---

template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &, const sequence<_ITEM> &);

template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &, const sdbi<_ITEM> &);

// ---

template<class _ITEM> range_type subseq(const sdbi<_ITEM> &, const sdbi<_ITEM> &, const range_type &);

template<class _ITEM> range_type subseq(const sdbi<_ITEM> &, const sequence<_ITEM> &, const range_type &);

template<class _ITEM> range_type subseq(const sequence<_ITEM> &, const sdbi<_ITEM> &, const range_type &);

// ---

template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &, const sdbi<_ITEM> &, const range_type &);

template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &, const sequence<_ITEM> &, const range_type &);

template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &, const sdbi<_ITEM> &, const range_type &);

// ---

template<class _ITEM> bool operator<(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator<(const sequence<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator<=(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator<=(const sequence<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator>(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator>(const sequence<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator>=(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator>=(const sequence<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator==(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator==(const sequence<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator!=(const sdbi<_ITEM> &, const sdbi<_ITEM> &);

template<class _ITEM> bool operator!=(const sequence<_ITEM> &, const sdbi<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Relations on SDB entries.
//
////////////////////////////////////////////////////////////////////////////////

/**
 * subseq 1.1
 */
template<class _ITEM> range_type subseq(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  return subseq(x, y, range_type(0, y.count.size() - x.count.size()));
}

/**
 * subseq 1.2
 */
template<class _ITEM> range_type subseq(const sdbi<_ITEM> &x, const sequence<_ITEM> &y) {
  return subseq(x, y, range_type(0, y.size() - x.count.size()));
}

/**
 * subseq 1.3
 */
template<class _ITEM> range_type subseq(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  return subseq(x, y, range_type(0, y.count.size() - x.size()));
}

/**
 * subseqeq 1.1
 */
template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  return subseqeq(x, y, range_type(0, y.count.size() - x.count.size()));
}

/**
 * subseqeq 1.2
 */
template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &x, const sequence<_ITEM> &y) {
  return subseqeq(x, y, range_type(0, y.size() - x.count.size()));
}

/**
 * subseqeq 1.3
 */
template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  return subseqeq(x, y, range_type(0, y.count.size() - x.size()));
}

/**
 * subseq 2.1
 */
template<class _ITEM> range_type subseq(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y, const range_type &r) {
  if (x.count.size() == y.count.size()) {
    return range_type(false);
  } else {
    return subseqeq(x, y, r);
  }
}

/**
 * subseq 2.2
 */
template<class _ITEM> range_type subseq(const sdbi<_ITEM> &x, const sequence<_ITEM> &y, const range_type &r) {
  if (x.count.size() == y.size()) {
    return range_type(false);
  } else {
    return subseqeq(x, y, r);
  }
}

/**
 * subseq 2.3
 */
template<class _ITEM> range_type subseq(const sequence<_ITEM> &x, const sdbi<_ITEM> &y, const range_type &r) {
  if (x.size() == y.count.size()) {
    return range_type(false);
  } else {
    return subseqeq(x, y, r);
  }
}

/**
 * subseqeq 2.1
 */
template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y, const range_type &r) {
  if (x.length > y.length || x.count.size() > y.count.size() - r.left) {
    return range_type(false);
  }
  size_t xf = 0;
  size_t xi = 0; // Count index.
  size_t yi = r.left; // Count index.
  size_t xo = 0; // Iptr offset.
  size_t yo = 0; // Iptr offset.
  if (yi) {
    yo = y.count[yi] + 1;
  }
  while (xi < x.count.size()) {
    if (yi == y.count.size()) {
      return range_type(false);
    }
    if (subseteq(x.iptr + xo, y.iptr + yo)) {
      xo += x.count[xi++] + 1;
    } // An itemset is terminated with 0.
    if (!xf) {
      if (!r[xi - 1]) {
        return range_type(false);
      }
      xf = xi;
    }
    yo += y.count[yi++] + 1; // An itemset is terminated with 0.
  }
  return range_type(xf - 1, yi - 1);
}

/**
 * subseqeq 2.2
 */
template<class _ITEM> range_type subseqeq(const sdbi<_ITEM> &x, const sequence<_ITEM> &y, const range_type &r) {
  if (x.length > y.length() || x.count.size() > y.size() - r.left) {
    return range_type(false);
  }
  size_t xf = 0;
  size_t xi = 0; // Count index.
  size_t xo = 0; // Iptr offset.
  size_t yi = r.left;
  yi = r.left;
  while (xi < x.count.size()) {
    if (yi == y.size()) {
      return range_type(false);
    }
    if (subseteq(x.iptr + xo, y[yi])) {
      xo += x.count[xi++] + 1;
    } // An itemset is terminated with 0.
    if (!xf) {
      if (!r[xi - 1]) {
        return range_type(false);
      }
      xf = xi;
    }
    ++yi;
  }
  return range_type(xf - 1, yi - 1);
}

/**
 * subseqeq 2.3
 */
template<class _ITEM> range_type subseqeq(const sequence<_ITEM> &x, const sdbi<_ITEM> &y, const range_type &r) {
  if (x.length() > y.length || x.size() > y.count.size() - r.left) {
    return range_type(false);
  }
  size_t xf = 0;
  size_t xi = 0;
  size_t yi = r.left; // Count index.
  size_t yo = 0; // Iptr offset.
  if (yi) {
    yo = y.count[yi] + 1;
  }
  while (xi < x.size()) {
    if (yi == y.count.size()) {
      return range_type(false);
    }
    if (subseteq(x[xi], y.iptr + yo)) {
      ++xi;
    }
    if (!xf) {
      if (!r[xi - 1]) {
        return range_type(false);
      }
      xf = xi;
    }
    yo += y.count[yi++] + 1; // An itemset is terminated with 0.
  }
  return range_type(xf - 1, yi - 1);
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator<(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  return x.length < y.length && x <= y;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator<(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  return x.length() < y.length && x <= y;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator<=(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  if (!x.length) {
    return true;
  }
  if (x.length > y.length || x.count.size() > y.count.size()) {
    return false;
  }
  size_t xi = 0, yi = 0; // Count index.
  size_t xo = 0, yo = 0; // Iptr offset.
  while (xi < x.count.size()) {
    if (yi == y.count.size()) {
      return false;
    }
    if (subseteq(x.iptr + xo, y.iptr + yo)) {
      xo += x.count[xi++] + 1;
    } // An itemset is terminated with 0.
    yo += y.count[yi++] + 1; // An itemset is terminated with 0.
  }
  return true;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator<=(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  if (!x.length()) {
    return true;
  }
  if (x.length() > y.length || x.size() > y.count.size()) {
    return false;
  }
  size_t xi = 0; // Itemset index.
  size_t yi = 0; // Count index.
  size_t yo = 0; // Iptr offset.
  while (xi < x.size()) {
    if (yi == y.count.size()) {
      return false;
    }
    if (subseteq(x[xi], y.iptr + yo)) {
      ++xi;
    }
    yo += y.count[yi++] + 1; // An itemset is terminated with 0.
  }
  return true;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator>(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  return x.length > y.length && x >= y;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator>(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  return x.length() > y.length && x >= y;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator>=(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  return y <= x;
}

/**
 * Test subsequence relation without range constraint.
 */
template<class _ITEM> bool operator>=(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  return y <= x;
}

/**
 * Test equivalence between two sequences.
 */
template<class _ITEM> bool operator==(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  if (x.length != y.length || x.count.size() != y.count.size()) {
    return false;
  }
  for (size_t i = 0; i < x.length; i++) {
    if (*x.iptr + i != *y.iptr + i) {
      return false;
    }
  }
  return true;
}

/**
 * Test equivalence between two sequences.
 */
template<class _ITEM> bool operator==(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  if (x.length() != y.length || x.size() != y.count.size()) {
    return false;
  }
  size_t xi = 0; // Itemset index.
  size_t yi = 0; // Count index.
  size_t yo = 0; // Iptr offset.
  while (xi < x.size()) {
    if (neq(x[xi++], y.iptr + yo)) {
      return false;
    }
    yo += y.count[yi++] + 1; // An itemset is terminated with 0.
  }
  return true;
}

/**
 * Test equivalence between two sequences.
 */
template<class _ITEM> bool operator!=(const sdbi<_ITEM> &x, const sdbi<_ITEM> &y) {
  return !(x == y);
}

/**
 * Test equivalence between two sequences.
 */
template<class _ITEM> bool operator!=(const sequence<_ITEM> &x, const sdbi<_ITEM> &y) {
  return !(x == y);
}

}

#endif /* _SDTL_RELATION_SDB_H_ */

