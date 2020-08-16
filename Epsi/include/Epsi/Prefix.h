/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Prefix.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_PREFIX_H_
#define _EPSI_PREFIX_H_

#include "sdtl/map.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// Prefix map for collecting last prefixes.
////

template<class ITEM, class SIZE>
struct Prefix : public map<ITEM, typename T<SIZE>::OFFSET> {
  Prefix() = default;
};

}

#endif /* _EPSI_PREFIX_H_ */
