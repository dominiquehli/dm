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
 * $Id: block.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_STREAM_BLOCK_CC_
#define _SDTL_STREAM_BLOCK_CC_

#include "util.hxx"
#include <list>
#include <set>
#include <vector>

namespace sdtl {

template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &, const set<_ITEM> &);

template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &, const std::list<_ITEM> &);

template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &, const std::set<_ITEM> &);

template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &, const std::vector<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Array operations.
//
////////////////////////////////////////////////////////////////////////////////

/**
 * A convenience wrapper for creating an block from iterative type.
 */
template<class _T, class _S> block<_T> &__make_block(block<_T> &__t, const _S &__s) {
  if (__t.size() != __s.size()) {
    __t.resize(__s.size());
  }
  typename _S::const_iterator it = __s.begin();
  for (size_t i = 0; it != __s.end(); i++, it++) {
    __t[i] = *it;
  }
  return __t;
}

/**
 * Array assignment.
 */
template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &x, const set<_ITEM> &y) {
  for (auto &&e : y) {
    x.put(e);
  }
  return x;
}

/**
 * Array assignment.
 */
template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &x, const std::list<_ITEM> &y) {
  return __make_block(x, y);
}

/**
 * Array assignment.
 */
template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &x, const std::set<_ITEM> &y) {
  return __make_block(x, y);
}

/**
 * Array assignment.
 */
template<class _ITEM> block<_ITEM> &operator<<(block<_ITEM> &x, const std::vector<_ITEM> &y) {
  return __make_block(x, y);
}

}

#endif /* _SDTL_STREAM_BLOCK_CC_ */
