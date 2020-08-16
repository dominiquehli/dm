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
 * $Id: vdb.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_VDB_H_
#define _SDTL_VDB_H_

#ifndef __sdtl_vdb_class
#define __sdtl_vdb_class
#endif

#include "block.h"
#include "db.h"
#include "stream.h"
#include "types.h"
#include "vector.h"

namespace sdtl {

/**
 * VDB Sequence Database.
 */
template<class _T> class vdb : public db<_T, vector<_T>> {
public:

  /**
   * Constructor.
   */
  vdb() : adb_(0), max_(0) {
  }

  /**
   * Constructor.
   */
  vdb(vdb<_T> &D) : adb_(&D), max_(0) {
  }

  /**
   * Constructor.
   */
  vdb(size_t n) : adb_(0), max_(0) {
  }

  /**
   * Destructor.
   */
  virtual ~vdb() {
    free_();
  }

  /**
   * Add an alias into the database.
   */
  void add(size_t n) {
    if (adb_) {
      alias_.add(n);
    }
  }

  /**
   * Add a sequence into the database.
   */
  void add(const vdbi <_T> &s) {
    if (adb_) {
      return;
    }
    //
    index * e = 0;
    //
    // Add an empty sequence.
    //
    if (!s.length) {
      list_.add(new index());
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
    b--;
    //
    // Prepare a VDB entry for the sequence.
    //
    e = new index();
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
    _T *iptr = e->iptr;
    _T *siptr = s.iptr;
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
    list_.add(e);
    if (e->length > longest_) {
      longest_ = e->length;
    }
    for (size_t i = 0; i < e->items.size(); i++) {
      this->counts.count(e->items[i]);
    }
  }

  /**
   * Add a sequence into the database.
   */
  void add(const sequence <_T> &s) {
    if (adb_) {
      return;
    }
    //
    index * e = 0;
    //
    // Add an empty sequence.
    //
    if (!s.length()) {
      list_.add(new index());
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
    // Prepare a VDB entry for the sequence.
    //
    e = new index();
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
    e->iptr = this->slices_[b]->data.ptr() + this->slices_[b]->offset;
    e->length = 0;
    _T *iptr = e->iptr;
    for (size_t i = 0; i < s.size(); i++) {
      const itemset <_T> &itemset = s[i]; // Avoid using s[i][j].
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
    list_.add(e);
    if (e->length > longest_) {
      longest_ = e->length;
    }
    for (size_t i = 0; i < e->items.size(); i++) {
      this->counts.count(e->items[i]);
    }
  }

  /**
   * Clear content.
   */
  void clear() {
    free_();
    alias_.clear();
    data_.clear();
    list_.clear();
    db<_T, sequence < _T> > ::clear();
  }

  /**
   * Find a sequence.
   */
  const index &find(size_t ID) const {
    if (adb_) {
      return adb_->data_[ID];
    }
    return data_[ID];
  }

  /**
   * Return longest sequence.
   */
  size_t longest() const {
    if (adb_) {
      return 0;
    }
    return longest_;
  }

  /**
   * Return size.
   */
  size_t size() const {
    if (adb_) {
      return alias_.size();
    }
    return list_.size();
  }

  /**
   * Update the item counter.
   */
  virtual void update() {
    if (adb_) {
      return;
    }
    this->counts.clear();
    for (size_t i = 0; i < list_.size(); i++) {
      for (size_t j = 0; j < list_[i]->items.size(); j++) {
        this->counts.count(list_[i]->items[j]);
      }
    }
  }

  /**
   * Access item pointer.
   */
  virtual inline const _T *operator()(size_t n, size_t p) const {
    if (adb_) {
      return adb_->list_[alias_[n]]->iptr + p;
    }
    return list_[n]->iptr + p;
  }

  /**
   * Access item pointer.
   */
  virtual inline _T *operator()(size_t n, size_t p) {
    if (adb_) {
      return adb_->list_[alias_[n]]->iptr + p;
    }
    return list_[n]->iptr + p;
  }

  /**
   * Access database index entry.
   */
  virtual inline const index &operator[](size_t n) const {
    if (adb_) {
      return *(adb_->list_[alias_[n]]);
    }
    return *list_[n];
  }

  /**
   * Access database index entry.
   */
  virtual inline index &operator[](size_t n) {
    if (adb_) {
      return *(adb_->list_[alias_[n]]);
    }
    return *list_[n];
  }

  /**
   * Database assignment.
   */
  inline vdb<_T> &operator=(const vdb<_T> &D) {
    if (adb_) {
      return *this;
    }
    copy_(D);
    return *this;
  }

private:

  /**
   * Database assignment.
   */
  void copy_(const vdb<_T> &D) {
    if (this != &D && !adb_) {
      if (this != &D) {
        for (size_t i = 0; i < D.size(); i++) {
          add(D[i]);
        }
      }
    }
  }

  /**
   * Free allocated memory.
   */
  void free_() {
    if (adb_) {
      return;
    }
    for (size_t i = 0; i < list_.size(); i++) {
      delete list_[i];
    }
  }

  /**
   * Alias database.
   */
  vdb<_T> *adb_;

  /**
   * Alias index.
   */
  block<size_t> alias_;

  /**
   * Data storage.
   */
  std::map<size_t, index *> data_;

  /**
   * List storage.
   */
  block<index *> list_;

  /**
   * Maximal vector ID.
   */
  size_t max_;

};

}

#endif /* _SDTL_VDB_H_ */
