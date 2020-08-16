/*-
 * Copyright (c) 2007-2012 Dominique Li <dominique.li@univ-tours.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: stream.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_STREAM_H_
#define _SDTL_STREAM_H_

#ifndef __sdtl_util_stream_functions
#define __sdtl_util_stream_functions
#endif

#include "dictionary.h"
#include "types.h"
#include <stdexcept>

#ifdef __sdtl_attribute_class
#include "stream/attribute.hxx"
#endif

#ifdef __sdtl_block_class

#include "stream/block.hxx"

#endif

#ifdef __sdtl_counter_class

#include "stream/counter.hxx"

#endif

#ifdef __sdtl_itemset_class

#include "stream/itemset.hxx"

#endif

#ifdef __sdtl_map_class

#include "stream/map.hxx"

#endif

#ifdef __sdtl_range_class
#include "stream/range.hxx"
#endif

#ifdef __sdtl_sdb_class

#include "stream/sdb.hxx"

#endif

#ifdef __sdtl_sequence_class

#include "stream/sequence.hxx"

#endif

#endif /* _SDTL_STREAM_H_ */
