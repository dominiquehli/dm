/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: ItemMap.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_ITEMMAP_H_
#define _EPSI_ITEMMAP_H_

#include <iostream>
#include "sdtl/map.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// Item map for building pseudo-projection.
////
template<class SIZE>
struct ItemMap : public map<typename T<SIZE>::ID, typename T<SIZE>::OFFSET> {
  ItemMap() = default;
};

template<class SIZE>
std::ostream &operator<<(std::ostream &out, const ItemMap<SIZE> &imap) {
  for (auto &&im : imap) {
    out << " " << im.first << ":" << im.second;
  }
  return out;
}

}

#endif /* _EPSI_ITEMMAP_H_ */
