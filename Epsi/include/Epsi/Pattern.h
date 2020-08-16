/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Pattern.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_PATTERN_H_
#define _EPSI_PATTERN_H_

#include "sdtl/sdb.h"
#include "sdtl/sequence.h"
#include "PseudoProjection.h"
#include "Setup.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// Generalized tree-based pattern data structure.
////
template<class ITEM, class SIZE>
struct Pattern {
  typedef typename T<SIZE>::ID ID;
  typedef typename T<SIZE>::ORDER ORDER;
  typedef typename PseudoProjection<ITEM, SIZE>::PAIR PAIR;
  const sdb<ITEM, SIZE> *SDB = nullptr;
  ITEM item = 0;
  Pattern<ITEM, SIZE> *link = nullptr;
  ORDER order = 0; // The order of current itemset in the pattern.
  SIZE support = 0;
  double weight = 1;
  Pattern() = default;
  explicit Pattern(const sdb<ITEM, SIZE> &);
  virtual ~Pattern() = default;
  Pattern<ITEM, SIZE> &fetch(const PAIR &);
  Pattern<ITEM, SIZE> &fetch(const PAIR &, Pattern<ITEM, SIZE> *);
  Pattern<ITEM, SIZE> &step(SIZE = 1);
  bool operator>(const ITEM &);
};

template<class ITEM, class SIZE>
Pattern<ITEM, SIZE>::Pattern(const sdb<ITEM, SIZE> &SDB) : SDB(&SDB) {
}

template<class ITEM, class SIZE>
Pattern<ITEM, SIZE> &Pattern<ITEM, SIZE>::fetch(const PAIR &pp) {
  item = pp.first;
  link = nullptr;
  order = 1;
  support = pp.second->size();
  weight = support;
  if (Setup::Weighted && SDB) {
    weight = 0;
    for (auto &&im : *pp.second) {
      weight += (*SDB)[im.first].weight;
    }
  }
  return *this;
}

template<class ITEM, class SIZE>
Pattern<ITEM, SIZE> &Pattern<ITEM, SIZE>::fetch(const PAIR &pp, Pattern<ITEM, SIZE> *p) {
  item = pp.first;
  link = p;
  order = p->order;
  support = pp.second->size();
  weight = support;
  if (Setup::Weighted && SDB) {
    weight = 0;
    for (auto &&im : *pp.second) {
      weight += (*SDB)[im.first].weight;
    }
  }
  return *this;
}

template<class ITEM, class SIZE>
Pattern<ITEM, SIZE> &Pattern<ITEM, SIZE>::step(SIZE n) {
  order += n;
  return *this;
}

template<class ITEM, class SIZE>
bool Pattern<ITEM, SIZE>::operator>(const ITEM &x) {
  Pattern<ITEM, SIZE> *p = this;
  while (p) {
    if (p->item == x) {
      return true;
    } else {
      p = p->link;
    }
  }
  return false;
}

////
// Produce a sequence from a pattern.
////
template<class ITEM, class SIZE>
sequence<ITEM> &operator<<(sequence<ITEM> &s, const Pattern<ITEM, SIZE> &p) {
  SIZE n;
  const Pattern<ITEM, SIZE> *sp;
  itemset<ITEM> t;
  s.resize(n = p.order);
  sp = &p;
  while (sp) {
    t.put(sp->item);
    if (!sp->link || sp->order > sp->link->order) {
      itemset<ITEM> &x = s[--n];
      if (x.size()) {
        x.clear();
      }
      // Reverse the itemset because of backward span.
      for (SIZE i = t.size(); i > 0; --i) {
        x.put(t[i - 1]);
      }
      t.clear();
      if (!options::itemset::list) {
        x.sort();
      }
    }
    sp = sp->link;
  }
  s.support = p.support;
  s.weight = p.weight;
  return s;
}

////
// Output a pattern.
////
template<class ITEM, class SIZE>
std::ostream &operator<<(std::ostream &out, const Pattern<ITEM, SIZE> &p) {
  sequence<ITEM> s;
  s << p;
  out << s;
  return out;
}

}

#endif /* _EPSI_PATTERN_H_ */
