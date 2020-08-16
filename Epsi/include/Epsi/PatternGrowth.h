/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: PatternGrowth.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_PATTERNGROWTH_H_
#define _EPSI_PATTERNGROWTH_H_

#include <iostream>
#include "sdtl/block.h"
#include "sdtl/itemset.h"
#include "sdtl/map.h"
#include "sdtl/measure.h"
#include "sdtl/sdb.h"
#include "sdtl/sequence.h"
#include "sdtl/stream.h"
#include "PDB.h"
#include "Pattern.h"
#include "Prefix.h"
#include "Projector.h"
#include "PseudoProjection.h"
#include "Setup.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// An implementation of the 1L-PrefixSpan algorithm based on Pattern-Growth.
////

template<class ITEM, class SIZE>
class PatternGrowth {
public:
  typedef Pattern<ITEM, SIZE> PATTERN;
  PatternGrowth(sdb<ITEM, SIZE> &, const measure<PATTERN> &);
  virtual ~PatternGrowth();
  void clear();
  size_t generate();
  size_t generate1L();
  void ignore1L(bool (*ignore)(const void *));
  block<PATTERN *> &patterns();
  block<PDB<ITEM, SIZE> *> &pdbs();
protected:
  void clear_(PDB<ITEM, SIZE> *);
  void free_();
  void patternGrowth_(PDB<ITEM, SIZE> *);
  void xdpo_(PDB<ITEM, SIZE> *);
  block<PATTERN *> PATS_; // All patterns.
  block<PATTERN *> PATX_; // Isolated patterns.
  block<PDB<ITEM, SIZE> *> PDB1_; // Level-1 patterns and projected databases.
  block<PDB<ITEM, SIZE> *> PDBS_; // All non-empty projected databases.
  block<PDB<ITEM, SIZE> *> PDBT_; // All projected databases. Free it.
  sdb<ITEM, SIZE> &S_; // Sequence set.
  bool clean_ = true; // All projected databases have been cleared.
  bool (*ignore1L_)(const void *); // Ignore a 1L pattern.
  const measure<PATTERN> &q_; // An interestingness measure->
};

//
// Constructor.
//
template<class ITEM, class SIZE>
PatternGrowth<ITEM, SIZE>::PatternGrowth(sdb<ITEM, SIZE> &S, const measure<PATTERN> &q)
    : S_(S), ignore1L_(Setup::Ignore1L), q_(q) {
}

//
// Destructor.
//
template<class ITEM, class SIZE>
PatternGrowth<ITEM, SIZE>::~PatternGrowth() {
  free_();
}

//
// Clear content.
//
template<class ITEM, class SIZE>
void PatternGrowth<ITEM, SIZE>::clear() {
  free_();
  PATS_.clear();
  PATX_.clear();
  PDB1_.clear();
  PDBS_.clear();
  PDBT_.clear();
}

//
// Generate all patterns.
//
template<class ITEM, class SIZE>
size_t PatternGrowth<ITEM, SIZE>::generate() {
  if (!clean_) {
    clear();
  }
  if (PDB1_.empty()) {
    generate1L();
  }
  for (auto &&pdb : PDB1_) {
    if (Setup::XDPO) { // Do DPO.
      xdpo_(pdb);
    }
    patternGrowth_(pdb);
    if (Setup::XDPO) { // Clear current PDB.
      delete pdb;
      pdb = nullptr;
    }
  }
  clean_ = false;
  return PDBT_.size();
}

//
// Generate level-1 patterns and projected databases.
//
// Traverse the sequence database once, put all items and generate
// all 1-length projected sequence databases. Then remove non-frequent
// items and projected databases.
//
template<class ITEM, class SIZE>
size_t PatternGrowth<ITEM, SIZE>::generate1L() {
  PATTERN pat(S_);
  PDB<ITEM, SIZE> *pdb;
  Prefix<ITEM, SIZE> pfx;
  Projector1L<ITEM, SIZE> prj;
  PseudoProjection<ITEM, SIZE> ppn;
  map<ITEM, SIZE> prn; // For item pruning.
  //
  // Generate projected database for each level-1 pattern.
  //
  for (auto &&e : S_) {
    if (!e->length || !prj(pfx, e->iptr)) {
      continue;
    }
    ppn.add(pfx, e->ID);
    pfx.clear();
  }
  //
  // Generate interesting level-1 patterns from projected databases.
  //
  for (auto &&pp : ppn) {
    if (!q_(pat.fetch(pp))) {
      ppn.free(pp.first);
      continue;
    }
    pdb = new PDB<ITEM, SIZE>(new PATTERN(pat), pp.second);
    PATS_.put(pdb->patt); // All patterns.
    PDB1_.put(pdb); // 1-length patterns.
    PDBT_.put(pdb); // All projected databases.
    prn.put(pdb->patt->item, 1);
  }
  //
  // Prune non-frequent items.
  //
  if (Setup::PruneItems) {
    for (auto &&e : S_) {
      ITEM *iptr = e->iptr;
      SIZE limit = 0;
      for (;;) {
        if (limit >= e->length) { // Anti-dead-loop.
          break;
        }
        if (!*iptr) {
          if (!*(iptr - 1)) {
            break;
          }
        } else {
          if (!prn.get(*iptr) && Setup::Blanc) {
            *iptr = *(ITEM *) Setup::Blanc;
          }
          ++limit;
        }
        ++iptr;
      }
    }
  }
  //
  // Done!
  //
  return PATS_.size();
}

//
// Register Ignore1L function.
//
template<class ITEM, class SIZE>
void PatternGrowth<ITEM, SIZE>::ignore1L(bool (*ignore)(const void *)) {
  ignore1L_ = ignore;
}

//
// Return all patterns.
//
template<class ITEM, class SIZE>
block<Pattern<ITEM, SIZE> *> &PatternGrowth<ITEM, SIZE>::patterns() {
  return PATS_;
}

//
// Return all projected databases.
//
template<class ITEM, class SIZE>
block<PDB<ITEM, SIZE> *> &PatternGrowth<ITEM, SIZE>::pdbs() {
  if (PDBS_.empty()) {
    for (auto &&p : PDBT_) {
      if (p->imap) {
        PDBS_.put(p);
      }
    }
  }
  return PDBS_;
}

//
// Clear a projected database->
//
template<class ITEM, class SIZE>
void PatternGrowth<ITEM, SIZE>::clear_(PDB<ITEM, SIZE> *pdb) {
  if (!pdb) {
    return;
  }
  if (pdb->imap) {
    delete pdb->imap;
    pdb->imap = 0;
  }
}

//
// Free allocated memory.
//
template<class ITEM, class SIZE>
void PatternGrowth<ITEM, SIZE>::free_() {
  for (SIZE i = 0; i < PATX_.size(); ++i) {
    delete PATX_[i];
    PATX_[i] = 0;
  }
  if (!Setup::XDPO) {
    for (SIZE i = 0; i < PDBT_.size(); ++i) {
      if (PDBT_[i]) {
        delete PDBT_[i];
      }
      PDBT_[i] = 0;
    }
  }
}

//
// Another implementation of the Pattern-Growth method.
//
// The PDB entry *p has been already associated with a projected database
// and is the last item of the current interesting prefix.
//
//
template<class ITEM, class SIZE>
void PatternGrowth<ITEM, SIZE>::patternGrowth_(PDB<ITEM, SIZE> *p) {
  PATTERN patt(S_);
  PDB<ITEM, SIZE> *pdb;
  Prefix<ITEM, SIZE> pfi, pfs; // pfi: itemset; pfs: sequence
  ProjectorKL<ITEM, SIZE> proj;
  PseudoProjection<ITEM, SIZE> ppi, pps; // ppi: itemset; pps: sequence
  //
  // Generate projected database for next pattern.
  //
  for (auto &&im : *p->imap) {
    if (!proj(pfi, pfs, S_[im.first].iptr, im.second)) {
      continue;
    }
    ppi.add(pfi, im.first);
    pfi.clear();
    pps.add(pfs, im.first);
    pfs.clear();
  }
  //
  // Generate interesting patterns from projected databases.
  //
  for (auto &&pp : ppi) {
    if (!q_(patt.fetch(pp, p->patt))) {
      ppi.free(pp.first);
      continue;
    }
    pdb = new PDB<ITEM, SIZE>(new PATTERN(patt), pp.second);
    if (pp.second == p->imap) {
      p->imap = nullptr;
    }
    PDBT_.put(pdb);
    PATS_.put(pdb->patt);
    if (Setup::XDPO) { // Enable PDO.
      xdpo_(pdb);
    }
    if (!Setup::MaxOrder || pdb->patt->order <= Setup::MaxOrder) {
      patternGrowth_(pdb);
    }
    if (Setup::XDPO) { // Clear current PDB.
      delete pdb;
    }
  }
  for (auto &&pp : pps) {
    if (!q_(patt.fetch(pp, p->patt).step())) {
      pps.free(pp.first);
      continue;
    }
    pdb = new PDB<ITEM, SIZE>(new PATTERN(patt), pp.second);
    PDBT_.put(pdb);
    PATS_.put(pdb->patt);
    if (Setup::XDPO) { // Enable PDO.
      xdpo_(pdb);
    }
    if (!Setup::MaxOrder || pdb->patt->order <= Setup::MaxOrder) {
      patternGrowth_(pdb);
    }
    if (Setup::XDPO) { // Clear current PDB.
      delete pdb;
    }
  }
  //
  // Done!
  //
}

//
// Direct pattern output.
//
template<class ITEM, class SIZE>
void PatternGrowth<ITEM, SIZE>::xdpo_(PDB<ITEM, SIZE> *pdb) {
  if (Setup::MaxOrder && pdb->patt->order > Setup::MaxOrder) {
    return;
  }
  if (Setup::MinOrder && pdb->patt->order < Setup::MinOrder) {
    return;
  }
  sequence<ITEM> s;
  s << *pdb;
  *Setup::XDPO << s;
  if (Setup::ShowID) {
    *Setup::XDPO << " | " << pdb->patt->support << " :";
    for (auto &&im : *pdb->imap) {
      *Setup::XDPO << " " << im.first;
    }
    *Setup::XDPO << " |";
  }
  *Setup::XDPO << std::endl;
}

}

/**
 * References:
 *   J. Pei, J. Han, B. Mortazavi-Asl, and H. Pinto. PrefixSpan: Mining
 *     sequential patterns efficiently by prefix-projected pattern growth.
 *     In ICDE, pages 215-224, 2001.
 */

#endif /* _EPSI_PATTERNGROWTH_H_ */
