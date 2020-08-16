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
 * $Id: counter.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_COUNTER_H_
#define _SDTL_COUNTER_H_

#ifndef __sdtl_counter_class
#define __sdtl_counter_class
#endif

#include <map>
#include "types.h"

namespace sdtl {

/**
 * Counter.
 */
template<class T_>
class counter {
public:
  counter() = default;
  counter(const counter<T_> &);
  virtual ~counter();
  inline typename std::map<T_, size_t>::iterator begin();
  inline typename std::map<T_, size_t>::const_iterator begin() const;
  void clear();
  inline typename std::map<T_, size_t>::iterator end();
  inline typename std::map<T_, size_t>::const_iterator end() const;
  counter<T_> &filter(size_t);
  size_t get(const T_ &);
  size_t get(const T_ &) const;
  size_t put(const T_ &);
  inline size_t size() const;
  inline size_t operator()(const T_ &);
  inline counter<T_> &operator=(const counter<T_> &);
  inline size_t operator[](const T_ &);
  inline size_t operator[](const T_ &) const;
private:
  void copy_(const counter<T_> &);
  void free_();
  std::map<T_, size_t> data_;
  std::map<size_t, counter<T_> *> filters_;
};

/**
 * Copy constructor.
 */
template<class T_>
counter<T_>::counter(const counter<T_> &c) {
  copy_(c);
}

/**
 * Destructor.
 */
template<class T_>
counter<T_>::~counter() {
  free_();
}

/**
 * For range based iteration: begin()
 */
template<class T_>
inline typename std::map<T_, size_t>::iterator counter<T_>::begin() {
  return data_.begin();
};

/**
 * For range based iteration: begin()
 */
template<class T_>
inline typename std::map<T_, size_t>::const_iterator counter<T_>::begin() const {
  return data_.begin();
};

/**
 * Clear content.
 */
template<class T_>
void counter<T_>::clear() {
  free_();
  data_.clear();
  filters_.clear();
}

/**
 * Range based iteration: end()
 */
template<class T_>
inline typename std::map<T_, size_t>::iterator counter<T_>::end() {
  return data_.end();
};

/**
 * Range based iteration: end()
 */
template<class T_>
inline typename std::map<T_, size_t>::const_iterator counter<T_>::end() const {
  return data_.end();
};

/**
 * Return a sub-counter for specified minimum count.
 */
template<class T_>
counter<T_> &counter<T_>::filter(size_t count) {
  auto &&it = filters_.find(count);
  if (it == filters_.end()) {
    auto *c = new counter<T_>();
    for (auto &e: data_) {
      if (e.second >= count) {
        c->data_[e.first] = e.second;
      }
    }
    filters_[count] = c;
    return *c;
  } else {
    return *it->second;
  }
}

/**
 * Get the count of an element.
 */
template<class T_>
size_t counter<T_>::get(const T_ &t) {
  auto &&it = data_.find(t);
  if (it == data_.end()) {
    return 0;
  } else {
    return it->second;
  }
}

/**
 * Get the count of an element.
 */
template<class T_>
size_t counter<T_>::get(const T_ &t) const {
  auto &&it = data_.find(t);
  if (it == data_.end()) {
    return 0;
  } else {
    return it->second;
  }
}

/**
 * Put an element to the counter.
 */
template<class T_>
size_t counter<T_>::put(const T_ &t) {
  auto &&it = data_.find(t);
  if (it == data_.end()) {
    data_.emplace(t, 1);
    return 1;
  } else {
    return ++it->second;
  }
}

/**
 * Return size.
 */
template<class T_>
inline size_t counter<T_>::size() const {
  return data_.size();
}

/**
 * Alias of put.
 */
template<class T_>
inline size_t counter<T_>::operator()(const T_ &t) {
  return this->put(t);
}

/**
 *
 */
template<class T_>
inline counter<T_> &counter<T_>::operator=(const counter<T_> &c) {
  copy_(c);
  return *this;
}

/**
 * Alias of get.
 */
template<class T_>
inline size_t counter<T_>::operator[](const T_ &t) {
  return this->get(t);
}

/**
 * Alias of get.
 */
template<class T_>
inline size_t counter<T_>::operator[](const T_ &t) const {
  return this->get(t);
}

/**
 *
 */
template<class T_>
void counter<T_>::copy_(const counter<T_> &c) {
  if (this != &c) {
    clear();
    data_ = c.data_;
    for (auto &sc: c.filters_) {
      filters_[sc.first] = new counter<T_>(*sc.second);
    }
  }
}

/**
 * Free allocated memory.
 */
template<class T_>
void counter<T_>::free_() {
  for (auto &f: filters_) {
    delete f.second;
  }
}

}

#endif /* _SDTL_COUNTER_H_ */
