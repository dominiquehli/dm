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
 * $Id: itemset.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_ITEMSET_H_
#define _SDTL_ITEMSET_H_

#ifndef __sdtl_itemset_class
#define __sdtl_itemset_class
#endif

#include <vector>
#include "sort.h"
#include "types.h"

namespace sdtl {

namespace options {
namespace itemset {
static bool list = false; // Treat itemset as list.
}
}

/**
 * Itemset is a set of items.
 */
template<class T_>
class itemset : public std::vector<T_> {
public:
  itemset<T_> &put(const T_ &);
  itemset<T_> &reset();
  itemset<T_> &reverse();
  itemset<T_> &sort();
  size_t ID = 0;
  size_t support = 0;
};

/**
 * Put an item.
 */
template<class T_>
itemset<T_> &itemset<T_>::put(const T_ &x) {
  this->emplace_back(x);
  return *this;
}

/**
 * Reset the itemset.
 */
template<class T_>
itemset<T_> &itemset<T_>::reset() {
  this->clear();
  support = 0;
  return *this;
}

/**
 * Reverse items.
 */
template<class T_>
itemset<T_> &itemset<T_>::reverse() {
  T_ x;
  auto data = this->data();
  auto n = this->size() - 1;
  for (size_t i = 0; i < data.size() / 2; ++i) {
    x = data[n - i];
    data[n - i] = data[i];
    data[i] = x;
  }
  return *this;
}

/**
 * Sort items.
 */
template<class T_>
itemset<T_> &itemset<T_>::sort() {
  msort(this->data(), this->size());
  return *this;
}

}

#endif /* _SDTL_ITEMSET_H_ */
