/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Support.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_SUPPORT_H_
#define _EPSI_SUPPORT_H_

#include "sdtl/measure.h"
#include "Pattern.h"
#include "Setup.h"

using namespace sdtl;

namespace Epsi {

template<class ITEM, class SIZE>
struct Support : public support<Pattern<ITEM, SIZE>> {
  bool operator()(const Pattern<ITEM, SIZE> &p) const {
    if (Setup::Weighted) {
      return (!this->threshold) ? false : p.weight >= this->threshold;
    } else {
      return (!this->threshold) ? false : p.support >= this->threshold;
    }
  }
};

}

#endif /* _EPSI_SUPPORT_H_ */
