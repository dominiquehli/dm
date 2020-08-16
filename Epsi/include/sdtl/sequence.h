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
 * $Id: sequence.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_SEQUENCE_H_
#define _SDTL_SEQUENCE_H_

#ifndef __sdtl_sequence_class
#define __sdtl_sequence_class
#endif

#include "itemset.h"
#include "set.h"
#include "types.h"
#include <vector>

namespace sdtl {

/**
 * Sequence is an ordered list of itemsets.
 */
template<class T_>
class sequence {
public:
  sequence() = default;
  sequence(const sequence<T_> &);
  sequence(const sequence<T_> &, size_t, size_t);
  virtual ~sequence();
  void add(const itemset<T_> &);
  const itemset<T_> &at(size_t) const;
  itemset<T_> &at(size_t);
  void clear();
  void copy(const sequence<T_> &, size_t, size_t);
  const std::set<T_> &items() const;
  size_t length() const;
  void refine(bool = true);
  void resize(size_t, itemset<T_> = itemset<T_>());
  size_t size() const;
  sequence<T_> &sort();
  sequence<T_> subsequence(size_t, size_t);
  inline const itemset<T_> &operator[](size_t) const;
  inline itemset<T_> &operator[](size_t);
  inline sequence<T_> &operator=(const sequence<T_> &);
  size_t ID = 0;
  size_t label = 0;
  double quality = 0;
  size_t support = 0;
  double weight = 1;
private:
  void copy_(const sequence<T_> &);
  void free_(size_t = 0);
  std::vector<itemset<T_> *> data_;
  std::set<T_> items_;
  size_t length_ = 0;
};

/**
 * Copy constructor.
 */
template<class T_>
sequence<T_>::sequence(const sequence<T_> &s) {
  copy_(s);
}

/**
 * Copy constructor.
 */
template<class T_>
sequence<T_>::sequence(const sequence<T_> &s, size_t p, size_t c) {
  copy(s, p, c);
}

/**
 * Destructor.
 */
template<class T_>
sequence<T_>::~sequence() {
  free_();
}

/**
 * Add an itemset to the sequence.
 */
template<class T_>
void sequence<T_>::add(const itemset<T_> &x) {
  data_.push_back(new itemset<T_>(x));
  for (size_t i = 0; i < x.size(); i++) {
    items_.insert(x[i]);
  }
  length_ += x.size();
}

/**
 * Access itemsets.
 */
template<class T_>
const itemset<T_> &sequence<T_>::at(size_t n) const {
  return *data_[n];
}

/**
 * Access itemsets.
 */
template<class T_>
itemset<T_> &sequence<T_>::at(size_t n) {
  return *data_[n];
}

/**
 * Clear content.
 */
template<class T_>
void sequence<T_>::clear() {
  free_();
  data_.clear();
  items_.clear();
  label = 0;
  length_ = 0;
  quality = 0;
  support = 0;
  weight = 1;
}

/**
 * Copy a consecutive subsequence from a sequence.
 */
template<class T_>
void sequence<T_>::copy(const sequence<T_> &s, size_t p, size_t c) {
  if (this != &s) {
    if (length_) {
      clear();
    }
    size_t n = (p + c <= s.size()) ? p + c : s.size();
    for (size_t i = p; i < n; i++) {
      add(s[i]);
    }
  }
}

/**
 * Return the cardinality items of the sequence.
 */
template<class T_>
const std::set<T_> &sequence<T_>::items() const {
  return items_;
}

/**
 * Return the number of items in a sequence.
 */
template<class T_>
size_t sequence<T_>::length() const {
  return length_;
}

/**
 * Refine sequence cardinality items.
 */
template<class T_>
void sequence<T_>::refine(bool sort) {
  items_.clear();
  length_ = 0;
  for (size_t i = 0; i < size(); i++) {
    if (sort) {
      data_[i]->sort();
    }
    for (size_t j = 0; j < data_[i]->size(); j++) {
      items_.add(data_[i]->at(j));
      ++length_;
    }
  }
}

/**
 * Resize the sequence.
 */
template<class T_>
void sequence<T_>::resize(size_t n, itemset<T_> x) {
  size_t size = data_.size();
  free_(n); // Important!
  data_.resize(n);
  for (size_t i = size; i < n; i++) {
    data_[i] = new itemset<T_>(x);
  }
  length_ = 0;
  for (size_t i = 0; i < n; i++) {
    length_ += data_[i]->size();
  }
}

/**
 * Return size.
 */
template<class T_>
size_t sequence<T_>::size() const {
  return data_.size();
}

/**
 * Sort items.
 */
template<class T_>
sequence<T_> &sequence<T_>::sort() {
  for (size_t i = 0; i < data_.size(); ++i) {
    data_[i].sort();
  }
  return *this;
}

/**
 * Return a consecutive subsequence of the sequence.
 */
template<class T_>
sequence<T_> sequence<T_>::subsequence(size_t p, size_t c) {
  if (p + c > data_.size()) {
    return sequence<T_>();
  }
  return sequence<T_>(*this, p, c);
}

/**
 * Access itemsets.
 */
template<class T_>
inline const itemset<T_> &sequence<T_>::operator[](size_t n) const {
  return at(n);
}

/**
 * Access itemsets.
 */
template<class T_>
inline itemset<T_> &sequence<T_>::operator[](size_t n) {
  return at(n);
}

/**
 * Sequence assignment.
 */
template<class T_>
inline sequence<T_> &sequence<T_>::operator=(const sequence<T_> &s) {
  copy_(s);
  return *this;
}

/**
 *
 */
template<class T_>
void sequence<T_>::copy_(const sequence<T_> &s) {
  if (this != &s) {
    if (length_) {
      clear();
    }
    label = s.label;
    quality = s.quality;
    support = s.support;
    weight = s.weight;
    items_ = s.items_;
    length_ = s.length_;
    data_.resize(s.size());
    for (size_t i = 0; i < s.size(); i++) {
      data_[i] = new itemset<T_>(s[i]);
    }
  }
}

/**
 * Free allocated memory.
 */
template<class T_>
void sequence<T_>::free_(size_t n) {
  for (size_t i = n; i < data_.size(); i++) {
    if (data_[i]) {
      delete data_[i];
    }
  }
}

}

#endif /* _SDTL_SEQUENCE_H_ */
