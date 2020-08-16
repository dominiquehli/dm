/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: PseudoProjection.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_PSEUDOPROJECTION_H_
#define _EPSI_PSEUDOPROJECTION_H_

#include "sdtl/map.h"
#include "ItemMap.h"
#include "Prefix.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// Pseudo-projection.
////

template<class ITEM, class SIZE>
struct PseudoProjection : public map<ITEM, ItemMap<SIZE> *> {
  typedef typename T<SIZE>::ID ID;
  typedef typename T<SIZE>::OFFSET OFFSET;
  typedef typename std::pair<ITEM, ItemMap<SIZE> *> PAIR;
  PseudoProjection() = default;
  virtual ~PseudoProjection() = default;
  void add(const Prefix<ITEM, SIZE> &, ID);
  void add(ITEM, ID, OFFSET);
  void free(ITEM);
  //std::vector<ItemMap<SIZE> *> maps();
};

template<class ITEM, class SIZE>
void PseudoProjection<ITEM, SIZE>::add(const Prefix<ITEM, SIZE> &prefix, ID id) {
  for (auto &&p : prefix) {
    add(p.first, id, p.second);
  }
}

template<class ITEM, class SIZE>
void PseudoProjection<ITEM, SIZE>::add(ITEM item, ID id, OFFSET offset) {
  ItemMap<SIZE> *&imap = this->operator[](item);
  if (!imap) {
    imap = new ItemMap<SIZE>();
  } // Will be released in PDB !!!
  imap->put(id, offset);
}

template<class ITEM, class SIZE>
void PseudoProjection<ITEM, SIZE>::free(ITEM item) {
  ItemMap<SIZE> *&imap = this->get(item);
  if (imap) {
    delete imap;
    imap = nullptr;
  }
}

//template<class ITEM, class SIZE>
//std::vector<ItemMap<SIZE> *> maps() {
//  return this->values();
//}

}

#endif /* _EPSI_PSEUDOPROJECTION_H_ */
