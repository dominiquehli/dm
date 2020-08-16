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
 * $Id: measure.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_MEASURE_H_
#define _SDTL_MEASURE_H_

#ifndef __sdtl_measure_class
#define __sdtl_measure_class
#endif

#include "types.h"

namespace sdtl {

/**
 * Interestingness measure.
 */
template<class T_>
struct measure {
  measure() = default;
  virtual ~measure() = default;
  virtual bool operator()(const T_ &) const = 0;
};

/**
 * Confidence of a pattern P_ containing the member `double confidence'.
 */
template<class P_>
struct confidence : measure<P_> {
  confidence() = default;
  explicit confidence(double);
  virtual ~confidence() = default;
  inline virtual bool operator()(const P_ &) const;
  inline confidence<P_> &operator=(const confidence<P_> &);
  double threshold = 0;
};

/**
 * Support of a pattern P_ containing the member `size_t support'.
 */
template<class P_>
struct support : measure<P_> {
  support() = default;
  explicit support(size_t);
  virtual ~support() = default;
  inline virtual bool operator()(const P_ &) const;
  inline support<P_> &operator=(const support<P_> &);
  size_t threshold = 0;
};

/**
 *
 */
template<class P_>
confidence<P_>::confidence(double threshold) : threshold(threshold) {
}

/**
 *
 */
template<class P_>
inline bool confidence<P_>::operator()(const P_ &p) const {
  return threshold == 0 ? false : p.confidence >= threshold;
}

/**
 *
 */
template<class P_>
inline confidence<P_> &confidence<P_>::operator=(const confidence<P_> &c) {
  if (this != &c) {
    threshold = c.threshold;
  }
  return *this;
}

/**
 *
 */
template<class P_>
support<P_>::support(size_t threshold) : threshold(threshold) {
}

/**
 *
 */
template<class P_>
inline bool support<P_>::operator()(const P_ &p) const {
  return (!threshold) ? false : p.support >= threshold;
}

/**
 *
 */
template<class P_>
inline support<P_> &support<P_>::operator=(const support<P_> &s) {
  if (this != &s) {
    threshold = s.threshold;
  }
  return *this;
}

}

#endif /* _SDTL_MEASURE_H_ */
