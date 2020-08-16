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
 * $Id: block.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_BLOCK_H_
#define _SDTL_BLOCK_H_

#ifndef __sdtl_block_class
#define __sdtl_block_class
#endif

#include <cstdlib>
#include <cstring>
#include "types.h"

namespace sdtl {

namespace options {
namespace block {
static size_t autosize = 1024;
static bool autostat = false;
}
}

/**
 * Variable-length block.
 */
template<class T_>
class block {
public:
  // For range based iteration.
  typedef T_ *iterator;
  typedef const T_ *const_iterator;
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  // End range based iteration.
  block();
  block(const block<T_> &);
  explicit block(size_t);
  block(size_t, T_);
  virtual ~block();
  void clean();
  void clear();
  T_ *data();
  bool empty() const;
  const T_ &get(size_t) const;
  T_ &get(size_t);
  void pack();
  block<T_> &put(const T_ &);
  block<T_> &resize(size_t);
  size_t size() const;
  inline const T_ &operator[](size_t) const;
  inline T_ &operator[](size_t);
private:
  void allocate_(size_t);
  void copy_(const block<T_> &);
  void free_();
  void reallocate_(size_t);
  size_t auto_ = options::block::autosize;
  T_ *data_ = nullptr;
  size_t real_ = 0;
  size_t size_ = 0;
};

/**
 * Range based iteration: begin
 */
template<class T_>
typename block<T_>::iterator block<T_>::begin() {
  return data_;
}

/**
 * Range based iteration: begin
 */
template<class T_>
typename block<T_>::const_iterator block<T_>::begin() const {
  return data_;
}

/**
 * Range based iteration: end
 */
template<class T_>
typename block<T_>::iterator block<T_>::end() {
  return data_ + size_;
}

/**
 * Range based iteration: end
 */
template<class T_>
typename block<T_>::const_iterator block<T_>::end() const {
  return data_ + size_;
}

/**
 * Constructor.
 */
template<class T_>
block<T_>::block() {
  allocate_(auto_);
}

/**
 * Copy constructor.
 */
template<class T_>
block<T_>::block(const block<T_> &b) : auto_(b.auto_) {
  if (b.size_) {
    allocate_(size_ = b.size_);
    memcpy((void *) data_, (void *) b.data_, size_ * sizeof(T_));
  }
}

/**
 * Constructor with size.
 */
template<class T_>
block<T_>::block(size_t n) : auto_(n) {
  allocate_(auto_);
}

/**
 * Constructor with size and default value.
 */
template<class T_>
block<T_>::block(size_t n, T_ x) : auto_(n) {
  allocate_(auto_);
  for (size_t i = 0; i < n; ++i) {
    put(x);
  }
}

/**
 * Destructor.
 */
template<class T_>
block<T_>::~block() {
  clear();
}

/**
 * Clean content.
 */
template<class T_>
void block<T_>::clean() {
  size_ = 0;
}

/**
 * Clear content.
 */
template<class T_>
void block<T_>::clear() {
  free_();
  size_ = 0;
}

/**
 * Return a pointer to the data.
 */
template<class T_>
T_ *block<T_>::data() {
  return data_;
}

/**
 * Empty block.
 */
template<class T_>
bool block<T_>::empty() const {
  return !size_;
}

/**
 * Access elements.
 */
template<class T_>
const T_ &block<T_>::get(size_t n) const {
  return data_[n];
}

/**
 * Access elements.
 */
template<class T_>
T_ &block<T_>::get(size_t n) {
  return data_[n];
}

/**
 * Pack the block.
 */
template<class T_>
void block<T_>::pack() {
  if (real_ > size_) {
    reallocate_(size_);
  }
}

/**
 * Put an element to the end of the block.
 *
 * This method may dynamically increase the real size of the block.
 */
template<class T_>
block<T_> &block<T_>::put(const T_ &t) {
  if (real_ == size_) {
    if (!data_) {
      allocate_(auto_);
    } else {
      reallocate_(real_ << 1u);
    }
  }
  data_[size_++] = t;
  return *this;
}

/**
 * Resize the block.
 */
template<class T_>
block<T_> &block<T_>::resize(size_t n) {
  if (n > real_) {
    reallocate_(n);
  }
  size_ = n;
  return *this;
}

/**
 * Return the number of elements in the block.
 */
template<class T_>
size_t block<T_>::size() const {
  return size_;
}

/**
 * Access elements.
 */
template<class T_>
inline const T_ &block<T_>::operator[](size_t n) const {
  return get(n);
}

/**
 * Access elements.
 */
template<class T_>
inline T_ &block<T_>::operator[](size_t n) {
  return get(n);
}

/**
 * Allocate memory for the block.
 */
template<class T_>
void block<T_>::allocate_(size_t n) {
  if (!data_) {
    data_ = new T_[real_ = n];
  }
}

/**
 * Array assignment.
 */
template<class T_>
void block<T_>::copy_(const block<T_> &x) {
  if (this != &x) {
    auto_ = x.auto_;
    if (data_) {
      clear();
    }
    if (x.size_) {
      allocate_(size_ = x.size_);
      memcpy((void *) data_, (void *) x.data_, size_ * sizeof(T_));
    } else {
      allocate_(auto_);
      size_ = 0;
    }
  }
}

/**
 * Free allocated memory.
 */
template<class T_>
void block<T_>::free_() {
  if (data_) {
    delete[] data_;
  }
  data_ = 0;
  real_ = 0;
}

/**
 * Reallocate memory of the block.
 */
template<class T_>
void block<T_>::reallocate_(size_t n) {
  //data_ = (T_*) realloc(data_, (real_ = n) * sizeof(T_));
  auto *temp = new T_[real_ = n];
  if (data_) {
    memcpy((void *) temp, (void *) data_, size_ * sizeof(T_));
    delete[] data_;
  }
  data_ = temp;
}

}

#endif /* _SDTL_BLOCK_H_ */
