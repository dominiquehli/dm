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

#ifndef _SDTL_STREAM_SDB_CC_
#define _SDTL_STREAM_SDB_CC_

#include "util.hxx"
#include <iostream>

namespace sdtl {

template<class ITEM_, class SIZE_>
std::ostream &operator<<(std::ostream &, const sdb<ITEM_, SIZE_> &);

template<class ITEM_, class SIZE_>
std::ostream &operator<<(std::ostream &, const sdbi<ITEM_, SIZE_> &);

template<class ITEM_, class SIZE_>
std::istream &operator>>(std::istream &, sdb<ITEM_, SIZE_> &);

////////////////////////////////////////////////////////////////////////////////
//
// SDB operations.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
template<class ITEM_, class SIZE_>
std::ostream &operator<<(std::ostream &out, const sdb<ITEM_, SIZE_> &D) {
  for (auto &&e : D) {
    out << *e << std::endl;
  }
  return out;
}

/**
 *
 */
template<class ITEM_, class SIZE_>
std::ostream &operator<<(std::ostream &out, const sdbi<ITEM_, SIZE_> &e) {
  // ITEM ... ITEM 0 ITEM ... ITEM 0 0
  // ITEM ... ITEM 0 ITEM ... ITEM 0 ... ITEM 0 0
  // ...
  ITEM_ *iptr = e.iptr;
  bool first = true;
  if (!io::output::raw) {
    out << "<";
  }
  for (;;) {
    if (!*iptr) { // 0 is found.
      if (!*(iptr - 1)) { // Backward checking.
        break;
      } // 00 implies the end of a sequence.
      if (!io::output::raw) {
        out << ")";
      } else {
        if (io::format::text) {
          out << " " << io::format::delim::text << " ";
        } else {
          out << " " << io::format::delim::item << " ";
        }
      }
      iptr++;
      first = true;
      continue;
    }
    if (!io::output::raw && first) {
      out << "(";
    } else if (!first) {
      out << " ";
    }
    if (io::format::text) {
      out << (*(dictionary<ITEM_> *) io::dico)(*iptr);
    } else {
      out << *iptr;
    }
    first = false;
    ++iptr;
  }
  if (!io::output::raw) {
    out << ">";
  } else {
    if (io::format::text) {
      out << io::format::delim::text;
    } else {
      out << io::format::delim::item;
    }
  }
  if (!io::output::clean) {
    if (io::output::quality || io::output::support || io::output::weight) {
      out << " " << io::format::delim::extra;
    }
    if (io::output::quality) {
      out << " " << e.quality;
    }
    if (io::output::support) {
      out << " " << e.support;
    }
    if (io::output::weight) {
      out << " " << e.weight;
    }
  }
  return out;
}

/**
 *
 */
template<class ITEM_, class SIZE_>
std::istream &operator>>(std::istream &in, sdb<ITEM_, SIZE_> &D) {
  size_t ID = 0;
  sequence<ITEM_> s;
  while (in.good()) {
    in >> s;
    if (s.size()) {
      s.ID = ++ID;
      D.add(s);
    }
  }
  return in;
}

}

#endif /* _SDTL_STREAM_SDB_CC_ */
