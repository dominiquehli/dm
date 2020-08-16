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
 * $Id: map.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_MAP_H_
#define _SDTL_MAP_H_

#ifndef __sdtl_map_class
#define __sdtl_map_class
#endif

#include <unordered_map>
#include <unordered_set>

namespace sdtl {

/**
 * Map is a std::unsorted_map wrapper for read-only access.
 */
template<class K_, class V_>
class map : public std::unordered_map<K_, V_> {
public:
  inline std::unordered_map<K_, V_> &add(const K_ &k, const V_ &v) {
    auto it = this->find(k);
    if (it == this->end()) {
      this->insert(std::make_pair(k, v));
    }
    return *this;
  }
  inline V_ &get(const K_ &k) {
    auto it = this->find(k);
    if (it == this->end()) {
      return zero_;
    } else {
      return it->second;
    }
  }
  inline const V_ &get(const K_ &k) const {
    auto it = this->find(k);
    if (it == this->end()) {
      return zero_;
    } else {
      return it->second;
    }
  }
  inline std::unordered_map<K_, V_> &put(const K_ &k, const V_ &v) {
    this->operator[](k) = v;
    return *this;
  }
private:
  V_ zero_{};
};

}

#endif /* _SDTL_MAP_H_ */
