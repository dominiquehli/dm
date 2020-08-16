/*-
 * Copyright (c) 2016 Dominique Li <dominique.li@univ-tours.fr>
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
 * $Id: sort.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_SORT_H_
#define _SDTL_SORT_H_

#ifndef __sdtl_util_sort_functions
#define __sdtl_util_sort_functions
#endif

#include "types.h"

namespace sdtl {

template<class T_> void msort(T_ *, size_t);
template<class T_> void msort(T_ *, size_t, bool);
template<class T_> void msort(T_ *, size_t, size_t, T_ *);
template<class T_> void merge(T_ *, size_t, size_t, size_t, T_ *);
template<class T_> void merge(T_ *, size_t, T_ *, size_t, T_ *);

#ifdef __sdtl_block_class

template<class T_> void sort(block<T_> &);
template<class T_> void sort(block<T_> &, bool);

template<class T_>
void sort(block<T_> &a) {
  sort(a, false);
}

template<class T_>
void sort(block<T_> &a, bool unique) {
  if (!a.size()) {
    return;
  }
  msort<T_>(a.front(), a.size(), unique);
  if (unique) {
    block<T_> b(a);
    T_ x = b[0];
    size_t aj = 1, bj = 1;
    for (; bj < b.size(); ++bj) {
      if (x == b[bj]) {
        continue;
      } else {
        a[aj++] = b[bj];
        x = b[bj];
      }
    }
    a.truncate(aj);
  }
}

#endif

template<class T_>
void msort(T_ *a, size_t length) {
  msort(a, length, false);
}

template<class T_>
void msort(T_ *a, size_t length, bool unique) {
  if (!length) {
    return;
  }
  T_ *tmp = new T_[length];
  msort<T_>(a, 0, length - 1, tmp);
  if (unique) {
    for (size_t i = 0; i < length; ++i) {
      *tmp++ = *a++;
    }
  }
  delete[] tmp;
}

template<class T_>
void msort(T_ a[], size_t first, size_t last, T_ *tmp) {
  if (first < last) {
    size_t middle = (first + last) / 2;
    msort(a, first, middle, tmp);
    msort(a, middle + 1, last, tmp);
    merge(a, first, middle, last, tmp);
  }
}

/**
 * Merge two sorted sub-blocks x[lo, mi] and x(mi, hi] to the block z.
 * The size of block z should be at least hi - lo.
 */
template<class T_>
void merge(T_ *x, size_t lo, size_t mi, size_t hi, T_ *z) {
  merge(x + lo, mi - lo + 1, x + mi + 1, hi - mi, z);
  for (size_t i = 0; i <= hi - lo; ++i) {
    *(x + lo + i) = *(z + i);
  }
}

/**
 * Merge two sorted blocks x of size m and y of size n to the block z.
 * The size of block z should be at least m + n.
 */
template<class T_>
void merge(T_ *x, size_t m, T_ *y, size_t n, T_ *z) {
  size_t i = 0, j = 0;
  for (; i < m && j < n;) {
    if (*x < *y) {
      *z++ = *x++;
      ++i;
    } else {
      *z++ = *y++;
      ++j;
    }
  }
  for (; i < m; ++i) {
    *z++ = *x++;
  }
  for (; j < n; ++j) {
    *z++ = *y++;
  }
}

}

#endif /* _SDTL_SORT_H_ */
