/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Projector.h 641 2020-08-16 15:23:50Z li $
 */

#ifndef _EPSI_PROJECTOR_H_
#define _EPSI_PROJECTOR_H_

#include "Prefix.h"
#include "Setup.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// 1-length pattern projector.
////
template<class ITEM, class SIZE>
struct Projector1L {
  size_t operator()(Prefix<ITEM, SIZE> &pfx, ITEM *iptr) {
    typename T<SIZE>::OFFSET ipos = 0;
    for (;;) {
      if (*iptr == 0) { // 0 is found.
        if (*(iptr - 1) == 0) { // Backward checking.
          break;
        } // 00 implies the end of a sequence.
        ++ipos;
        ++iptr; // 0 implies the end of an itemset.
        continue;
      }
      pfx.add(*iptr, ipos); // Add non-existing items only.
      ++ipos;
      ++iptr;
    }
    return pfx.size();
  }
};

////
// K-length pattern projector.
////
template<class ITEM, class SIZE>
struct ProjectorKL {
  size_t operator()(Prefix<ITEM, SIZE> &pfi, Prefix<ITEM, SIZE> &pfs, ITEM *iptr, typename T<SIZE>::OFFSET ipos) {
    // pfc: itemset
    // pfs: sequence
    bool again = false; // Item appears again in another itemset.
    bool first = true; // Within the first itemset.
    typename T<SIZE>::OFFSET gap = 0;
    iptr += ipos++;
    ITEM *t = iptr++; // Store current item into *t and start from next one.
    bool ignore = false;
    ITEM *xxx = nullptr;
    if (Setup::Blanc && Setup::PruneItems) {
      ignore = true;
      xxx = (ITEM *) Setup::Blanc;
    }
    for (;;) {
      if (*iptr == 0) { // 0 is found.
        if (*(iptr - 1) == 0) { // Backward checking.
          break;
        } // 00 implies the end of a sequence.
        again = false;
        if (first) {
          first = false;
        }
        ++ipos;
        ++iptr; // 0 implies the end of an itemset.
        ++gap;
        continue;
      }
      if (!first) {
        if (Setup::MaxDistance && gap > Setup::MaxDistance) {
          break;
        }
        if (Setup::MinDistance && gap < Setup::MinDistance) {
          ++ipos;
          ++iptr;
          continue;
        }
        if (*(iptr - 1) == *t) {
          again = true;
        }
        if (again) {
          if (ignore) {
            if (*iptr != *xxx) {
              pfi.add(*iptr, ipos);
            }
          } else {
            pfi.add(*iptr, ipos);
          }
        }
        if (ignore) {
          if (*iptr != *xxx) {
            pfs.add(*iptr, ipos);
          }
        } else {
          pfs.add(*iptr, ipos);
        }
      } else {
        if (ignore) {
          if (*iptr != *xxx) {
            pfi.add(*iptr, ipos);
          }
        } else {
          pfi.add(*iptr, ipos);
        }
      }
      ++ipos;
      ++iptr;
    }
    return pfs.size() + pfi.size();
  }
};

}

#endif /* _EPSI_PROJECTOR_H_ */
