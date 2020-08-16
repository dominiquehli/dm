/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Setup.h 641 2020-08-16 15:23:50Z li $
 */

#ifndef _EPSI_SETUP_H_
#define _EPSI_SETUP_H_

#define EPSI_BLANC "@@Epsi::Setup::Blanc@@"

#include <ostream>

namespace Epsi {
namespace Setup {
void *Blanc = nullptr; // Ignore this item.
static bool (*Ignore1L)(const void *) = nullptr; // To ignore 1L patterns.
static size_type MaxDistance = 0; // Maximum distance between two itemsets.
static size_type MaxLength = 0; // Maximum number of itemsets.
static size_type MaxOrder = 0; // Maximum number of itemsets.
static size_type MinDistance = 0; // Minimum distance between two itemsets.
static size_type MinLength = 0; // Minimum number of itemsets.
static size_type MinOrder = 0; // Minimum number of itemsets.
static bool PruneItems = true; // Prune non-frequent items.
static bool ShowID = false; // Show sequence IDs in output patterns.
static double Support = 0.0; // Minimum support value.
static bool Weighted = false; // Mining patterns from weighted sequences.
static std::ostream *XDPO = nullptr; // Direct pattern output stream.
}
}

#endif /* _EPSI_SETUP_H_ */
