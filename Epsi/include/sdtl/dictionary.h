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
 * $Id: dictionary.h 627 2020-08-15 14:44:04Z li $
 */

#ifndef _SDTL_DICTIONARY_H_
#define _SDTL_DICTIONARY_H_

#ifndef __sdtl_dictionary_class
#define __sdtl_dictionary_class
#endif

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace sdtl {

/**
 * Dictionary.
 */
template<class T_>
class dictionary {
public:
  dictionary() = default;
  explicit dictionary(std::string);
  virtual ~dictionary() = default;
  void clear();
  void dump(std::ostream & = std::cerr);
  inline std::string &label(T_);
  inline T_ &value(const std::string &);
  inline std::string &operator()(T_);
  inline T_ &operator[](const std::string &);
  inline dictionary<T_> &operator=(const dictionary<T_> &);
private:
  void copy_(const dictionary<T_> &);
  std::string def_;
  T_ idx_ = 0;
  std::unordered_map<std::string, T_> map_;
  std::unordered_map<T_, std::string> rev_;
};

/**
 *
 */
template<class T_>
dictionary<T_>::dictionary(std::string def) : def_(std::move(def)) {
}

/**
 *
 */
template<class T_>
void dictionary<T_>::dump(std::ostream &o) {
  for (auto &&p : map_) {
    o << p.first << " => " << p.second << std::endl;
  }
}

/**
 *
 */
template<class T_>
void dictionary<T_>::clear() {
  idx_ = 0;
  map_.clear();
  rev_.clear();
}

/**
 *
 */
template<class T_>
inline std::string &dictionary<T_>::label(T_ n) {
  return rev_[n];
}

/**
 *
 */
template<class T_>
inline T_ &dictionary<T_>::value(const std::string &label) {
  return (*this)[label];
}

/**
 *
 */
template<class T_>
inline std::string &dictionary<T_>::operator()(T_ n) {
  return rev_[n];
}

/**
 *
 */
template<class T_>
inline T_ &dictionary<T_>::operator[](const std::string &label) {
  auto it = map_.find(label);
  if (it == map_.end()) {
    map_[label] = ++idx_;
    rev_[idx_] = label;
    return idx_;
  }
  return it->second;
}

/**
 *
 */
template<class T_>
inline dictionary<T_> &dictionary<T_>::operator=(const dictionary<T_> &d) {
  copy_(d);
  return *this;
}

/**
 *
 */
template<class T_>
void dictionary<T_>::copy_(const dictionary<T_> &d) {
  if (this != &d) {
    clear();
    def_ = d.def_;
    idx_ = d.idx_;
    map_ = d.map_;
    rev_ = d.rev_;
  }
}

}

#endif /* _SDTL_DICTIONARY_H_ */
