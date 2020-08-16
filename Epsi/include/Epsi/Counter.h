/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Counter.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_COUNTER_H_
#define _EPSI_COUNTER_H_

#include "Prefix.h"
#include "Setup.h"
#include "Type.h"

using namespace sdtl;

namespace Epsi {

////
// 1-length pattern projector.
////
template<class ITEM, class SIZE>
struct Counter1L {
  SIZE operator()(ITEM item, Prefix<ITEM, SIZE> &pfx, ITEM *iptr, typename T<SIZE>::OFFSET) {
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
      if (*iptr == item) {
        pfx.add(*iptr, ipos); // Add non-existing items only.
        ++ipos;
        ++iptr;
      }
    }
    return pfx.size();
  }
};

////
// X-length pattern projector.
////
template<class ITEM, class SIZE>
struct CounterKLC {
  SIZE operator()(ITEM item, Prefix<ITEM, SIZE> &cpfc, ITEM *iptr, typename T<SIZE>::OFFSET ipos) {
    bool again = false; // Item appears again in another itemset.
    bool first = true; // Within the first itemset.
    typename T<SIZE>::OFFSET gap = 0;
    iptr += ipos++;
    ITEM *t = iptr++; // Store current item into *t and start from next one.
    for (;;) {
      if (*iptr == 0) { // 0 is found.
        if (*(iptr - 1) == 0) { // Backward checking.
          break;
        } // 00 implies the end of a sequence.
        again = false;
        if (first) {
          first = false;
        }
        ipos++;
        iptr++; // 0 implies the end of an itemset.
        gap++;
        continue;
      }
      if (!first) {
        if (Setup::MaxGap > 0 && gap > Setup::MaxGap) {
          break;
        }
        if (gap < Setup::MinGap) {
          continue;
        }
        if (*(iptr - 1) == *t) {
          again = true;
        }
        if (again) {
          if (*iptr == item) {
            cpfc.add(*iptr, ipos);
          }
        }
      } else {
        if (*iptr == item) {
          cpfc.add(*iptr, ipos);
        }
      }
      ipos++;
      iptr++;
    }
    return cpfc.size();
  }
};

template<class ITEM, class SIZE>
struct CounterKL {
  SIZE operator()(ITEM item, Prefix<ITEM, SIZE> &cpfn, ITEM *iptr, typename T<SIZE>::OFFSET ipos) {
    typename T<SIZE>::OFFSET gap = 0;
    iptr += ipos++;
    ++iptr;
    for (;;) {
      if (*iptr == 0) { // 0 is found.
        if (*(iptr - 1) == 0) { // Backward checking.
          break;
        } // 00 implies the end of a sequence.
        ipos++;
        iptr++; // 0 implies the end of an itemset.
        gap++;
        continue;
      }
      if (Setup::MaxGap > 0 && gap > Setup::MaxGap) {
        break;
      }
      if (gap < Setup::MinGap) {
        continue;
      }
      if (*iptr == item) {
        cpfn.add(*iptr, ipos);
      }
      ipos++;
      iptr++;
    }
    return cpfn.size();
  }
};

}

#endif /* _EPSI_COUNTER_H_ */
