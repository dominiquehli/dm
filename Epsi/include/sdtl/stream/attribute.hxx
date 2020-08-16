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
 * $Id: attribute.hxx 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_STREAM_ATTRIBUTE_CC_
#define _SDTL_STREAM_ATTRIBUTE_CC_

#include "util.hxx"
#include <iostream>
#include <sstream>
#include <string>

namespace sdtl {

template<class _N, class _V> std::ostream &operator<<(std::ostream &, const attribute <_N, _V> &);

template<class _N, class _V> std::istream &operator>>(std::istream &, attribute <_N, _V> &);

////////////////////////////////////////////////////////////////////////////////
//
// Attribute operations.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
template<class _N, class _V> std::ostream &operator<<(std::ostream &out, const attribute <_N, _V> &a) {
  if (io::output::simple) {
    out << a.value;
  } else {
    out << a.name << io::format::delim::attribute << a.value;
  }
  return out;
}

/**
 *
 */
template<class _N, class _V> std::istream &operator>>(std::istream &in, attribute <_N, _V> &a) {
  if (!in.good()) {
    throw stream_error();
  }
  std::string str;
  in >> str;
  std::stringstream end;
  end << io::format::delim::item;
  if (str == end.str() || str == "") {
    a = 0;
    return in;
  }
  size_t delim = str.find(io::format::delim::attribute);
  if (delim == -1) {
    throw format_error();
  }
  std::istringstream name(str.substr(0, delim));
  std::istringstream value(str.substr(delim + 1, str.size()));
  name >> a.name;
  value >> a.value;
  return in;
}

}

#endif /* _SDTL_STREAM_ATTRIBUTE_CC_ */
