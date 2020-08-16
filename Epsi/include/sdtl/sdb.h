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
 * $Id: sdb.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_SDB_H_
#define _SDTL_SDB_H_

#ifndef __sdtl_sdb_class
#define __sdtl_sdb_class
#endif

#include <iostream>
#include "block.h"
#include "db.h"
#include "itemset.h"
#include "map.h"
#include "sequence.h"
#include "set.h"
#include "types.h"

namespace sdtl {

/**
 * SDB index.
 */
template<class T_, class S_>
struct sdbi : public dbi<T_> {
  size_t ID = 0;
  set<T_> items;
  std::vector<S_> count;
  size_t support = 0;
  double weight = 0;
  sdbi() : ID(0) {
  };
};

/**
 * SDB Sequence Database.
 */
template<class T_, class S_>
class sdb : public db<T_, sequence<T_>> {
public:
  // For range based iteration.
  typedef typename block<sdbi<T_, S_> *>::iterator iterator;
  typedef typename block<sdbi<T_, S_> *>::const_iterator const_iterator;
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  // End range based iteration.
  typedef sdbi<T_, S_> index;
  sdb() = default;
  explicit sdb(size_t);
  virtual ~sdb();
  void add(const sdbi<T_, S_> &);
  void add(const sequence<T_> &);
  void clear();
  void dump(std::ostream & = std::cerr);
  const index &find(size_t) const;
  size_t longest() const;
  size_t size() const;
  virtual void update();
  virtual inline const T_ *operator()(size_t, size_t) const;
  virtual inline T_ *operator()(size_t, size_t);
  virtual inline const index &operator[](size_t) const;
  virtual inline index &operator[](size_t);
private:
  void free_();
  map<S_, index *> data_; // Data storage.
  block<index *> index_; // Index storage.
  size_t longest_ = 0; // Longest sequence.
  size_t max_ = 0; // Maximal sequence ID.
};

/**
 * Range based iteration: begin
 */
template<class T_, class S_>
typename sdb<T_, S_>::iterator sdb<T_, S_>::begin() {
  return index_.begin();
}

/**
 * Range based iteration: begin
 */
template<class T_, class S_>
typename sdb<T_, S_>::const_iterator sdb<T_, S_>::begin() const {
  return index_.begin();
}

/**
 * Range based iteration: end
 */
template<class T_, class S_>
typename sdb<T_, S_>::iterator sdb<T_, S_>::end() {
  return index_.end();
}

/**
 * Range based iteration: end
 */
template<class T_, class S_>
typename sdb<T_, S_>::const_iterator sdb<T_, S_>::end() const {
  return index_.end();
}

/**
 * Constructor.
 */
template<class T_, class S_>
sdb<T_, S_>::sdb(size_t n) : db<T_, sequence<T_>>(n) {
}

/**
 * Destructor.
 */
template<class T_, class S_>
sdb<T_, S_>::~sdb() {
  free_();
}

/**
 * Add a sequence into the database.
 */
template<class T_, class S_>
void sdb<T_, S_>::add(const sdbi<T_, S_> &s) {
  //
  // Add an empty sequence.
  //
  if (!s.length) {
    index_.put(new index());
    return;
  }
  //
  // A too long sequence.
  //
  if (s.length > this->size_) {
    return;
  }
  //
  // Prepare data slice.
  //
  size_t b = this->prepare_(s.length + s.count.size() + 1); // Append 0.
  if (!b) {
    return;
  }
  --b;
  //
  // Prepare a SDB entry for the sequence.
  //
  auto *e = new index();
  if (s.ID) {
    e->ID = s.ID;
    if (e->ID > max_) {
      max_ = e->ID;
    }
  } else {
    e->ID = ++max_;
  }
  e->count = s.count;
  e->iptr = this->slices_[b]->data.ptr() + this->slices_[b]->offset;
  e->length = 0;
  T_ *iptr = e->iptr;
  T_ *siptr = s.iptr;
  for (size_t i = 0; i < s.count.size(); i++) {
    for (size_t j = 0; j < s.count[i]; j++) {
      *iptr++ = *siptr++;
      ++e->length;
    }
    ++siptr;
    *iptr++ = 0; // Append 0 to the end of itemset.
  }
  *iptr = 0; // Append 00 to the end of sequence.
  e->items = s.items;
  e->label = s.label;
  e->quality = s.quality;
  e->support = s.support;
  e->weight = s.weight;
  this->slices_[b]->offset += s.length + s.count.size() + 1;
  data_[e->ID] = e; // Sequences with same ID will be overwritten!
  index_.put(e);
  if (e->length > longest_) {
    longest_ = e->length;
  }
  for (size_t i = 0; i < e->items.size(); i++) {
    this->counts.put(e->items[i]);
  }
}

/**
 * Add a sequence into the database.
 */
template<class T_, class S_>
void sdb<T_, S_>::add(const sequence<T_> &s) {
  //
  // Add an empty sequence.
  //
  if (!s.length()) {
    index_.put(new index());
    return;
  }
  //
  // A too long sequence.
  //
  if (s.length() > this->size_) {
    return;
  }
  //
  // Prepare data slice.
  //
  size_t b = this->prepare_(s.length() + s.size() + 1); // Append 0.
  if (!b) {
    return;
  }
  b--;
  //
  // Prepare a SDB entry for the sequence.
  //
  auto *e = new index();
  if (s.ID) {
    e->ID = s.ID;
    if (e->ID > max_) {
      max_ = e->ID;
    }
  } else {
    e->ID = ++max_;
  }
  e->count.resize(s.size());
  for (size_t i = 0; i < s.size(); i++) {
    e->count[i] = s[i].size();
  }
  e->iptr = this->slices_[b]->block.data() + this->slices_[b]->offset;
  e->length = 0;
  T_ *iptr = e->iptr;
  for (size_t i = 0; i < s.size(); i++) {
    const itemset<T_> &itemset = s[i]; // Avoid using s[i][j].
    for (size_t j = 0; j < itemset.size(); j++) {
      *iptr++ = itemset[j];
      ++e->length;
    }
    *iptr++ = 0; // Append 0 to the end of itemset.
  }
  *iptr = 0; // Append 00 to the end of sequence.
  e->items << s.items();
  e->label = s.label;
  e->quality = s.quality;
  e->support = s.support;
  e->weight = s.weight;
  this->slices_[b]->offset += s.length() + s.size() + 1;
  data_[e->ID] = e; // Sequences with same ID will be overwritten!
  index_.put(e);
  if (e->length > longest_) {
    longest_ = e->length;
  }
  for (auto &&i : e->items) {
    this->counts.put(i);
  }
}

/**
 * Clear content.
 */
template<class T_, class S_>
void sdb<T_, S_>::clear() {
  free_();
  data_.clear();
  index_.clear();
  db<T_, sequence<T_>>::clear();
}

/**
 * Dump raw data.
 */
template<class T_, class S_>
void sdb<T_, S_>::dump(std::ostream &o) {
  for (auto &&s : this->slices_) {
    size_t z = 0;
    for (auto &&x : s->block) {
      if (!x) {
        ++z;
      } else {
        z = 0;
      }
      if (z > 3) {
        break;
      }
      o << x << " ";
    }
    o << std::endl;
  }
}

/**
 * Find a sequence.
 */
template<class T_, class S_>
const typename sdb<T_, S_>::index &sdb<T_, S_>::find(size_t ID) const {
  return data_[ID];
}

/**
 * Return longest sequence.
 */
template<class T_, class S_>
size_t sdb<T_, S_>::longest() const {
  return longest_;
}

/**
 * Return size.
 */
template<class T_, class S_>
size_t sdb<T_, S_>::size() const {
  return index_.size();
}

/**
 * Update the item counter.
 */
template<class T_, class S_>
void sdb<T_, S_>::update() {
  this->counts.clear();
  for (size_t i = 0; i < index_.size(); i++) {
    for (auto &&x : index_[i]->items) {
      this->counts.put(x);
    }
  }
}

/**
 * Access item pointer.
 */
template<class T_, class S_>
inline const T_ *sdb<T_, S_>::operator()(size_t n, size_t p) const {
  return index_[n]->iptr + p;
}

/**
 * Access item pointer.
 */
template<class T_, class S_>
inline T_ *sdb<T_, S_>::operator()(size_t n, size_t p) {
  return index_[n]->iptr + p;
}

/**
 * Access database index entry.
 */
template<class T_, class S_>
inline const typename sdb<T_, S_>::index &sdb<T_, S_>::operator[](size_t n) const {
  return *(data_.get(n));
}

/**
 * Access database index entry.
 */
template<class T_, class S_>
inline typename sdb<T_, S_>::index &sdb<T_, S_>::operator[](size_t n) {
  return *(data_.get(n));
}

/**
 * Free allocated memory.
 */
template<class T_, class S_>
void sdb<T_, S_>::free_() {
  for (size_t i = 0; i < index_.size(); i++) {
    delete index_[i];
  }
}

}

#endif /* _SDTL_SDB_H_ */
