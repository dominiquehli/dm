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
 * $Id: itemset.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_RELATION_ITEMSET_H_
#define _SDTL_RELATION_ITEMSET_H_

#include <iostream>

namespace sdtl {

//---

template<class _ITEM> bool eq(const _ITEM *, const _ITEM *);

template<class _ITEM> bool eq(const _ITEM *, const itemset<_ITEM> &);

template<class _ITEM> bool eq(const itemset<_ITEM> &, const _ITEM *);

template<class _ITEM> bool eq(const itemset<_ITEM> &, const itemset<_ITEM> &);

// ---

template<class _ITEM> bool neq(const _ITEM *, const _ITEM *);

template<class _ITEM> bool neq(const _ITEM *, const itemset<_ITEM> &);

template<class _ITEM> bool neq(const itemset<_ITEM> &, const _ITEM *);

template<class _ITEM> bool neq(const itemset<_ITEM> &, const itemset<_ITEM> &);

// ---

template<class _ITEM> bool subset(const _ITEM *, const _ITEM *);

template<class _ITEM> bool subset(const _ITEM *, const itemset<_ITEM> &);

template<class _ITEM> bool subset(const itemset<_ITEM> &x, const _ITEM *);

template<class _ITEM> bool subset(const itemset<_ITEM> &, const itemset<_ITEM> &);

// ---

template<class _ITEM> bool subseteq(const _ITEM *, const _ITEM *);

template<class _ITEM> bool subseteq(const _ITEM *, const itemset<_ITEM> &);

template<class _ITEM> bool subseteq(const itemset<_ITEM> &, const _ITEM *);

template<class _ITEM> bool subseteq(const itemset<_ITEM> &, const itemset<_ITEM> &);

// ---

template<class _ITEM> bool operator==(const itemset<_ITEM> &, const itemset<_ITEM> &);

template<class _ITEM> bool operator!=(const itemset<_ITEM> &, const itemset<_ITEM> &);

template<class _ITEM> bool operator<(const itemset<_ITEM> &, const itemset<_ITEM> &);

template<class _ITEM> bool operator<=(const itemset<_ITEM> &, const itemset<_ITEM> &);

template<class _ITEM> bool operator>(const itemset<_ITEM> &, const itemset<_ITEM> &);

template<class _ITEM> bool operator>=(const itemset<_ITEM> &, const itemset<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Relations on itemsets.
//
////////////////////////////////////////////////////////////////////////////////

/**
 * eq 1
 */
template<class _ITEM> bool eq(const _ITEM *x, const _ITEM *y) {
  if (!*x) {
    return !*y;
  } // If x is empty, then y must be empty.
  while (*x) {
    if (!*y) {
      return false;
    }
    if (*x != *y) {
      return false;
    }
    ++x;
    ++y;
  }
  return !*y;
}

/**
 * eq 2
 */
template<class _ITEM> bool eq(const _ITEM *x, const itemset<_ITEM> &y) {
  if (!*x) {
    return !y.size();
  } // If x is empty, then y must be empty.
  size_t yi = 0;
  while (*x) {
    if (yi == y.size()) {
      return false;
    }
    if (*x != y[yi]) {
      return false;
    }
    ++x;
    ++yi;
  }
  return yi == y.size();
}

/**
 * eq 3
 */
template<class _ITEM> bool eq(const itemset<_ITEM> &x, const _ITEM *y) {
  return eq(y, x);
}

/**
 * eq 4
 */
template<class _ITEM> bool eq(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
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
 * neq 1
 */
template<class _ITEM> bool neq(const _ITEM *x, const _ITEM *y) {
  return !eq(x, y);
}

/**
 * neq 2
 */
template<class _ITEM> bool neq(const _ITEM *x, const itemset<_ITEM> &y) {
  return !eq(x, y);
}

/**
 * neq 3
 */
template<class _ITEM> bool neq(const itemset<_ITEM> &x, const _ITEM *y) {
  return !eq(x, y);
}

/**
 * neq 4
 */
template<class _ITEM> bool neq(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  return !eq(x, y);
}

/**
 * subset 1
 */
template<class _ITEM> bool subset(const _ITEM *x, const _ITEM *y) {
  if (!*y) {
    return false;
  } // Anyway, y must not be empty.
  if (!*x) {
    return true;
  } // Empty set is a subset of any non-empty set.
  bool sub = false;
  while (*x) {
    if (!*y)
      return false;
    if (*x == *y)
      ++x;
    else if (!sub)
      sub = true;
    ++y;
  }
  return sub;
}

/**
 * subset 2
 */
template<class _ITEM> bool subset(const _ITEM *x, const itemset<_ITEM> &y) {
  if (!y.size()) {
    return false;
  } // Anyway, y must not be empty.
  if (!*x) {
    return true;
  } // Empty set is a subset of any non-empty set.
  bool sub = false;
  size_t yi = 0;
  while (*x) {
    if (yi == y.size()) // |x| > |y|
      return false;
    if (*x == y[yi])
      ++x;
    else if (!sub)
      sub = true;
    ++yi;
  }
  return sub;
}

/**
 * subset 3
 */
template<class _ITEM> bool subset(const itemset<_ITEM> &x, const _ITEM *y) {
  if (!*y) {
    return false;
  } // Anyway, y must not be empty.
  if (!x.size()) {
    return true;
  } // Empty set is a subset of any non-empty set.
  bool sub = false;
  size_t xi = 0;
  while (xi < x.size()) {
    if (!*y) // |x| > |y|
      return false;
    if (x[xi] == *y)
      ++xi;
    else if (!sub)
      sub = true;
    ++y;
  }
  return sub;
}

/**
 * subset 4
 */
template<class _ITEM> bool subset(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  if (x.size() >= y.size()) {
    return false;
  }
  return subseteq(x, y);
}

/**
 * subseteq 1
 */
template<class _ITEM> bool subseteq(const _ITEM *x, const _ITEM *y) {
  if (!*x) {
    return true;
  } // Anyway, an empty set is subseteq to any set.
  if (!*y) {
    return false;
  } // If x is not empty, then y must not be empty.
  while (*x) {
    if (!*y)
      return false;
    if (*x == *y)
      ++x;
    ++y;
  }
  return true;
}

/**
 * subseteq 2
 */
template<class _ITEM> bool subseteq(const _ITEM *x, const itemset<_ITEM> &y) {
  if (!*x) {
    return true;
  } // Anyway, an empty set is subseteq to any set.
  if (!y.size()) {
    return false;
  } // If x is not empty, then y must not be empty.
  size_t yi = 0;
  while (*x) {
    if (yi == y.size())
      return false;
    if (*x == y[yi])
      ++x;
    ++yi;
  }
  return true;
}

/**
 * subseteq 3
 */
template<class _ITEM> bool subseteq(const itemset<_ITEM> &x, const _ITEM *y) {
  if (!x.size()) {
    return true;
  } // Anyway, an empty set is subseteq to any set.
  if (!*y) {
    return false;
  } // If x is not empty, then y must not be empty.
  size_t xi = 0;
  while (xi < x.size()) {
    if (!*y) // |x| > |y|
      return false;
    if (x[xi] == *y)
      ++xi;
    ++y;
  }
  return true;
}

/**
 * subseteq 4
 */
template<class _ITEM> bool subseteq(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  if (!x.size()) {
    return true;
  } // Anyway, an empty set is subseteq to any set.
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
 * Test equivalence of two itemsets.
 */
template<class _ITEM> bool operator==(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  if (!x.size() && !y.size()) {
    return true;
  }
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
 * Test difference of two itemsets.
 */
template<class _ITEM> bool operator!=(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  return !(x == y);
}

/**
 * Test subset relation of two itemsets.
 */
template<class _ITEM> bool operator<(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  return x.size() < y.size() && subseteq(x, y);
}

/**
 * Test subset-equivalence relation of two itemsets.
 */
template<class _ITEM> bool operator<=(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  return subseteq(x, y);
}

/**
 * Test superset relation of two itemsets.
 */
template<class _ITEM> bool operator>(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  return subset(y, x);
}

/**
 * Test superset-equivalence relation of two itemsets.
 */
template<class _ITEM> bool operator>=(const itemset<_ITEM> &x, const itemset<_ITEM> &y) {
  return subseteq(y, x);
}

}

#endif /* _SDTL_RELATION_ITEMSET_H_ */
