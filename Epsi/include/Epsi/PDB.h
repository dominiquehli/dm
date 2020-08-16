/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: PDB.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_PDB_H_
#define _EPSI_PDB_H_

#include <iostream>
#include "sdtl/sequence.h"
#include "sdtl/stream.h"
#include "ItemMap.h"
#include "Pattern.h"

using namespace sdtl;

namespace Epsi {

////
// Projected database of a pattern.
////
template<class ITEM, class SIZE>
struct PDB {
  ItemMap <SIZE> *imap; // Pseudo-projected database of pattern.
  Pattern <ITEM, SIZE> *patt; // Linked list of pattern.
  PDB(Pattern <ITEM, SIZE> *pa, ItemMap <SIZE> *pr) : patt(pa), imap(pr) {
  }
  virtual ~PDB() {
    if (patt) {
      delete patt;
    }
    if (imap) {
      delete imap;
    }
  }
  void discard() {
    if (imap) {
      delete imap;
    }
    imap = nullptr;
  }
};

//
// Produce a sequence from a projected database.
//
template<class ITEM, class SIZE>
sequence<ITEM> &operator<<(sequence<ITEM> &s, const PDB<ITEM, SIZE> &pdb) {
  return s << *pdb.patt;
}

//
// Output a projected database.
//
template<class ITEM, class SIZE>
std::ostream &operator<<(std::ostream &out, const PDB<ITEM, SIZE> &pdb) {
  out << io::dico->label(pdb.patt->item) << " = " << *pdb.imap;
  return out;
}

}

#endif /* _EPSI_PDB_H_ */
