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
 * $Id: types.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _SDTL_TYPES_H_
#define _SDTL_TYPES_H_

#ifndef _sdtl_types
#define _sdtl_types
#endif

#include <cstdint>

namespace sdtl {

/**
 * Large    -- 64-bit, not really needed
 * Normal   -- 32-bit, the most general size
 * Small    -- 16-bit, to save memory usage
 * Tiny     -- 8-bit, to save memory usage
 */

/**
 * By default, item is an unsigned 32-bit integer.
 */
typedef unsigned int _item_type_; // 32-bit item, 0 to 4,294,967,295

/**
 * Other item sizes.
 */
typedef unsigned long long int large_item_type_; // 64-bit item
typedef unsigned short int small_item_type_; // 16-bit item
typedef unsigned char tiny_item_type_; // 8-bit item

/**
 * Compilation options for specifying item size. Just for funny.
 */

#ifdef SDTL_TINY_ITEM_ONLY
typedef tiny_item_type_ item_type;
typedef tiny_item_type_ large_item_type;
typedef tiny_item_type_ small_item_type;
typedef tiny_item_type_ tiny_item_type;
#else
#ifdef SDTL_SMALL_ITEM_ONLY
typedef small_item_type_ item_type;
typedef small_item_type_ large_item_type;
typedef small_item_type_ small_item_type;
typedef small_item_type_ tiny_item_type;
#else
#ifdef SDTL_LARGE_ITEM_ONLY
typedef large_item_type_ item_type;
typedef large_item_type_ large_item_type;
typedef large_item_type_ small_item_type;
typedef large_item_type_ tiny_item_type;
#else
typedef _item_type_ item_type;
typedef large_item_type_ large_item_type;
typedef small_item_type_ small_item_type;
typedef tiny_item_type_ tiny_item_type;
#endif
#endif
#endif

/**
 * By default, size_t container size is an unsigned 32-bit integer.
 */
typedef unsigned int _size_type_; // 32-bit size, 0 to 4,294,967,295

/**
 * Other container sizes.
 */
typedef unsigned long long int large_size_type_; // 64-bit size
typedef unsigned short int small_size_type_; // 16-bit size
typedef unsigned char tiny_size_type_; // 8b, 0 to 255

/**
 * Compilation options for specifying container size.
 */

#ifdef SDTL_TINY_SIZE_ONLY
typedef tiny_size_type_ size_type;
typedef tiny_size_type_ large_size_type;
typedef tiny_size_type_ small_size_type;
typedef tiny_size_type_ tiny_size_type;
#else
#ifdef SDTL_SMALL_SIZE_ONLY
typedef small_size_type_ size_type;
typedef small_size_type_ large_size_type;
typedef small_size_type_ small_size_type;
typedef small_size_type_ tiny_size_type;
#else
#ifdef SDTL_LARGE_SIZE_ONLY
typedef large_size_type_ size_type;
typedef large_size_type_ large_size_type;
typedef large_size_type_ small_size_type;
typedef large_size_type_ tiny_size_type;
#else
typedef _size_type_ size_type;
typedef large_size_type_ large_size_type;
typedef small_size_type_ small_size_type;
typedef tiny_size_type_ tiny_size_type;
#endif
#endif
#endif

/**
 * Maximum values of integer.
 */
#define SDTL_ITEM         0xffffffffUL
#define SDTL_LARGE_ITEM   0xffffffffffffffffULL
#define SDTL_SMALL_ITEM   65535
#define SDTL_TINY_ITEM    255
#define SDTL_SIZE         0xffffffffUL
#define SDTL_LARGE_SIZE   0xffffffffffffffffULL
#define SDTL_SMALL_SIZE   65535
#define SDTL_TINY_SIZE    255

}

#endif /* _SDTL_TYPES_H_ */
