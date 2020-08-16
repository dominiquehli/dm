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
 * $Id: range.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_STREAM_RANGE_CC_
#define _SDTL_STREAM_RANGE_CC_

#include "util.hxx"
#include <iostream>

namespace sdtl {

template<class _T> std::ostream &operator<<(std::ostream &, const range <_T> &);

template<class _T> std::istream &operator>>(std::istream &, range <_T> &);

////////////////////////////////////////////////////////////////////////////////
//
// Range operations.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
template<class _T> std::ostream &operator<<(std::ostream &out, const range <_T> &r) {
  out << "[" << r.min << ".." << r.max << "]";
  return out;
}

/**
 *
 */
template<class _T> std::istream &operator>>(std::istream &in, range <_T> &r) {
  if (in.good()) {
    in >> r.min;
  } else {
    throw stream_error();
  }
  if (in.good()) {
    in >> r.max;
  } else {
    throw stream_error();
  }
  return in;
}

}

#endif /* _SDTL_STREAM_RANGE_CC_ */
