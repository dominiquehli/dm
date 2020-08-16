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

#ifndef _SDTL_STREAM_SEQUENCE_CC_
#define _SDTL_STREAM_SEQUENCE_CC_

#include "itemset.hxx"
#include "util.hxx"
#include <iostream>

namespace sdtl {

template<class _ITEM> std::ostream &operator<<(std::ostream &, const sequence<_ITEM> &);

template<class _ITEM> std::istream &operator>>(std::istream &, sequence<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Sequence operations.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
template<class _ITEM> std::ostream &operator<<(std::ostream &out, const sequence<_ITEM> &s) {
  if (s.size()) {
    if (!io::output::raw) {
      out << "<";
    }
    for (size_t i = 0; i < s.size(); i++) {
      out << s[i];
      if (io::output::raw) {
        if (io::format::text) {
          out << " " << io::format::delim::text << " ";
        } else {
          out << " " << io::format::delim::item << " ";
        }
      }
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
      if (io::output::label || io::output::quality || io::output::support || io::output::weight) {
        out << " " << io::format::delim::extra;
      }
      if (io::output::label) {
        out << " " << s.label;
      }
      if (io::output::quality) {
        out << " " << s.quality;
      }
      if (io::output::support) {
        out << " " << s.support;
      }
      if (io::output::weight) {
        out << " " << s.weight;
      }
    }
  }
  return out;
}

/**
 *
 */
template<class _ITEM> std::istream &operator>>(std::istream &in, sequence<_ITEM> &s) {
  if (!in.good()) {
    throw stream_error();
  }
  _ITEM i;
  itemset<_ITEM> x;
  if (s.size()) {
    s.clear();
  }
  while (in >> x) {
    // A sequence must be terminated with an empty itemset.
    if (x.size()) {
      s.add(x);
    } else {
      if (!s.size()) {
        return in;
      }
      // End of input sequence.
      if (io::input::label || io::input::quality || io::input::support || io::input::weight) {
        char extra;
        in >> extra; // Read io::format::delim::extra.
      }
      if (io::input::label) {
        in >> s.label;
      } // Read label value.
      if (io::input::quality) {
        in >> s.quality;
      } // Read quality value.
      if (io::input::support)
        in >> s.support; // Read support value.
      if (io::input::weight)
        in >> s.weight; // Read weight value.
      return in;
    }
  }
  if (s.size()) {
    throw format_error();
  }
  return in;
}

}

#endif /* _SDTL_STREAM_SEQUENCE_CC_ */
