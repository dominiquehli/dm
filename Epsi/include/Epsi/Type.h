/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Type.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _EPSI_TYPE_H_
#define _EPSI_TYPE_H_

#include "sdtl/types.h"

namespace Epsi {

template<class SIZE>
struct T {
  typedef SIZE OFFSET;
  typedef SIZE ORDER;
  typedef SIZE ID;
  typedef SIZE TID;
};

}

#endif /* _EPSI_TYPE_H_ */
