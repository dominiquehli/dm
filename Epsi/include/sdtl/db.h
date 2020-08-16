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
 * $Id: db.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _SDTL_DB_H_
#define _SDTL_DB_H_

#ifndef __sdtl_db_class
#define __sdtl_db_class
#endif

#include "block.h"
#include "counter.h"
#include "dictionary.h"
#include "types.h"

namespace sdtl {

/**
 * Dictionary.
 */
static dictionary<large_item_type> dico;

/**
 * Database index entry.
 */
template<class T_> struct dbi {
  T_ *iptr = 0;
  size_t label = 0;
  size_t length = 0;
  double quality = 0;
};

/**
 * Database.
 */
template<class T_, class C_>
class db {
public:
  typedef dbi<T_> index;
  /**
   * Database slice.
   */
  struct slice {
#ifndef SDTL_DB_BLOCK_MEMORY
    static const size_t memory = 16384;
#else
    static const size_t memory = SDTL_DB_BLOCK_MEMORY;
#endif
#ifndef SDTL_DB_BLOCK_NUMBER
    static const size_t number = 1024;
#else
    static const size_t number = SDTL_DB_BLOCK_NUMBER;
#endif
    block<T_> block;
    size_t offset = 0;
    explicit slice(size_t n) : offset(1) {
      block.resize(n);
    }
  };
  db();
  explicit db(size_t);
  virtual ~db();
  virtual void add(const C_ &) = 0;
  virtual void clear();
  // Update statistics information of the database.
  virtual void update() = 0;
  // Access item pointer.
  virtual const T_ *operator()(size_t, size_t) const = 0;
  // Access database index entry.
  virtual const index &operator[](size_t) const = 0;
  counter<T_> counts;
protected:
  virtual void free_();
  size_t prepare_(size_t);
  block<slice *> slices_;
  size_t size_ = 0;
};

template<class T_, class C_>
db<T_, C_>::db() : size_(slice::memory * 1024 / sizeof(T_) + 2) {
}

/**
 * Constructor.
 */
template<class T_, class C_>
db<T_, C_>::db(size_t n) : size_(n * 1024 / sizeof(T_) + 2) {
}

/**
 * Destructor.
 */
template<class T_, class C_>
db<T_, C_>::~db() {
  for (size_t i = 0; i < slices_.size(); i++) {
    delete slices_[i];
  }
}

/**
 * Clear content.
 */
template<class T_, class C_>
void db<T_, C_>::clear() {
  free_();
  slices_.clear();
  counts.clear();
}

/**
 * Free allocated memory.
 */
template<class T_, class C_>
void db<T_, C_>::free_() {
  for (size_t i = 0; i < slices_.size(); i++) {
    delete slices_[i];
  }
}

/**
 * Prepare a data slice.
 */
template<class T_, class C_>
size_t db<T_, C_>::prepare_(size_t length) {
  size_t b = 0;
  for (; b < slices_.size(); b++) {
    if (slices_[b]->block.size() - slices_[b]->offset > length) {
      return b + 1;
    }
  }
  // OK, we have enough space in this slice.
  if (b == slices_.size()) {
    if (slices_.size() < slice::number) {
      slices_.put(new slice(size_));
    } else {
      return 0;
    }
    // A slice starts with 1 and ends with 00.
    slices_[b]->block[0] = 0;
    slices_[b]->block[1] = 0;
    slices_[b]->block[size_ - 2] = 0;
    slices_[b]->block[size_ - 1] = 0;
  }
  return b + 1;
}

}

#endif /* _SDTL_DB_H_ */
