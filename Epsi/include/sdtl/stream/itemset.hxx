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

#ifndef _SDTL_STREAM_ITEMSET_CC_
#define _SDTL_STREAM_ITEMSET_CC_

#include "../dictionary.h"
#include "util.hxx"
#include <iostream>
#include <list>
#include <set>
#include <string>

namespace sdtl {

template<class _ITEM> std::ostream &operator<<(std::ostream &, const itemset<_ITEM> &);

template<class _ITEM> std::istream &operator>>(std::istream &, itemset<_ITEM> &);

////////////////////////////////////////////////////////////////////////////////
//
// Itemset operations.
//
////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
template<class _ITEM>
std::ostream &operator<<(std::ostream &out, const itemset<_ITEM> &x) {
  if (io::format::text && !io::dico) {
    throw dictionary_error();
  }
  std::list<std::string> xl;
  typename std::list<std::string>::iterator it;
  // Item list disables sorted output.
  if (io::output::sort && !options::itemset::list) {
    for (size_t i = 0; i < x.size(); ++i) {
      xl.push_back(io::dico->label(x[i]));
    }
    xl.sort();
    if (xl.size() != x.size()) {
      throw format_error();
    }
  }
  if (!io::output::raw) {
    out << "(";
  }
  if (x.size()) {
    it = xl.begin();
    if (io::format::text) {
      if (io::output::sort && !options::itemset::list) {
        out << *it;
      } else {
        out << io::dico->label(x[0]);
      }
    } else {
      out << x[0];
    }
    ++it;
    for (size_t i = 1; i < x.size(); ++i, ++it) {
      if (io::format::text) {
        if (io::output::sort && !options::itemset::list) {
          out << " " << *it;
        } else {
          out << " " << io::dico->label(x[i]);
        }
      } else {
        out << " " << x[i];
      }
    }
  }
  if (!io::output::raw) {
    out << ")";
  }
  return out;
}

/**
 *
 */
template<class _ITEM>
std::istream &operator>>(std::istream &in, itemset<_ITEM> &x) {
  if (io::format::text && !io::dico) {
    throw dictionary_error();
  }
  if (!in.good()) {
    throw stream_error();
  }
  _ITEM i;
  std::set<_ITEM> xs;
  typename std::set<_ITEM>::iterator it;
  if (x.size()) {
    x.clear();
  }
  if (io::format::text) {
    std::string t; // Text data input.
    while (in >> t) {
      if (t != io::format::delim::text) {
        if (options::itemset::list) {
          x.put(io::dico->value(t));
        } else {
          xs.insert(io::dico->value(t));
        }
      } else {
        if (options::itemset::list) {
          return in;
        } else {
          break;
        }
      }
    }
    if (!options::itemset::list) {
      for (it = xs.begin(); it != xs.end(); ++it) {
        x.put(*it);
      }
      return in;
    }
  } else {
    while (in >> i) {
      if (i != io::format::delim::item) {
        if (options::itemset::list) {
          x.put(i);
        } else {
          xs.insert(i);
        }
      } else {
        if (!options::itemset::list) {
          for (it = xs.begin(); it != xs.end(); ++it) {
            x.put(*it);
          }
        }
        return in;
      }
    }
  }
  if (x.size()) {
    throw format_error();
  }
  return in;
}

}

#endif /* _SDTL_STREAM_ITEMSET_CC_ */
