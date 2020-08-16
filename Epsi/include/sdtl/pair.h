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
 * $Id: pair.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_PAIR_H_
#define _SDTL_PAIR_H_

#ifndef __sdtl_pair_class
#define __sdtl_pair_class
#endif

namespace sdtl {

/**
 *
 */
template<class F_, class S_> struct pair {
  /**
   *
   */
  pair() = default;
  /**
   *
   */
  pair(F_ first, S_ second) : first(first), second(second) {
  }
  /**
   *
   */
  pair(const pair<F_, S_> &p) : first(p.first), second(p.second) {
  }
  /**
   *
   */
  virtual ~pair() = default;
  /**
   *
   */
  pair<F_, S_> &operator=(const pair<F_, S_> &p) {
    if (this != &p) {
      first = p.first;
      second = p.second;
    }
    return *this;
  }
  /**
   *
   */
  F_ first;
  /**
   *
   */
  S_ second;

};

/**
 * Operator == for two pairs.
 */
template<class F_, class S_> bool operator==(const pair<F_, S_> &a, const pair<F_, S_> &b) {
  return a.first == b.first && a.second == b.second;
}

/**
 * Operator < for two pairs.
 */
template<class F_, class S_> bool operator<(const pair<F_, S_> &a, const pair<F_, S_> &b) {
  if (a.first == b.first) {
    return a.second < b.second;
  } else {
    return a.first < b.first;
  }
}

}

#endif /* _SDTL_PAIR_H_ */
