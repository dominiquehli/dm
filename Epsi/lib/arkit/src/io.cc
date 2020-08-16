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
 * $Id: io.cc 625 2020-08-15 14:38:40Z li $
 */

#include "arkit/io.h"

namespace arkit {

/**
 *
 */
IO::IO() {
  is_.push_back(&std::cin);
  os_.push_back(&std::cout);
}

/**
 *
 */
IO::~IO() {
  clear();
}

/**
 *
 */
void IO::clear() {
  for (size_t i = 1; i < is_.size(); i++) {
    if (is_[i]) {
      if (is_[i] != &std::cin) {
        delete is_[i];
      }
      is_[i] = nullptr;
    }
  }
  is_.clear();
  for (size_t i = 1; i < os_.size(); i++) {
    if (os_[i]) {
      if (os_[i] != &std::cout) {
        delete os_[i];
      }
      os_[i] = nullptr;
    }
  }
  os_.clear();
}

/**
 *
 */
void IO::close(std::istream &ist) {
  for (size_t i = 1; i < is_.size(); i++) {
    if (is_[i] == &ist) {
      if (is_[i] != &std::cin) {
        delete is_[i];
      }
      is_[i] = nullptr;
    }
  }
}

/**
 *
 */
void IO::close(std::ostream &ost) {
  for (size_t i = 1; i < os_.size(); i++) {
    if (os_[i] == &ost) {
      if (os_[i] != &std::cout) {
        delete os_[i];
      }
      os_[i] = nullptr;
    }
  }
}

/**
 *
 */
std::ostream &IO::error(const char *filename) {
  if (error_) {
    error_->close();
    delete error_;
  }
  error_ = new std::ofstream(filename);
  return *error_;
}

/**
 *
 */
std::ostream &IO::error() {
  return error_ ? *error_ : std::cerr;
}

/**
 *
 */
std::istream &IO::in() {
  if (is_.size() > 1) {
    return *is_[is_.size() - 1];
  } else {
    return std::cin;
  }
}

/**
 *
 */
std::istream &IO::in(size_t n) {
  if (!n) {
    return std::cin;
  }
  return *is_[n];
}

/**
 *
 */
std::istream &IO::input(const char *filename) {
  if (!*filename) {
    is_.push_back(&std::cin);
    return std::cin;
  }
  auto *ifs = new std::ifstream(filename);
  if (!ifs->is_open()) {
    throw IOError("Cannot open file for reading");
  } else {
    is_.push_back(ifs);
  }
  return *ifs;
}

/**
 *
 */
std::istream &IO::input(const std::string &filename) {
  return input(filename.c_str());
}

/**
 *
 */
std::ostream &IO::out() {
  if (os_.size() > 1) {
    return *os_[os_.size() - 1];
  } else {
    return std::cout;
  }
}

/**
 *
 */
std::ostream &IO::out(size_t n) {
  if (!n) {
    return std::cout;
  }
  return *os_[n];
}

/**
 *
 */
std::ostream &IO::output(const char *filename) {
  return output(filename, std::ios::out);
}

/**
 *
 */
std::ostream &IO::output(const std::string &filename) {
  return output(filename.c_str(), std::ios::out);
}

/**
 *
 */
std::ostream &IO::output(const char *filename, std::ios::openmode mode) {
  if (!*filename) {
    os_.push_back(&std::cout);
    return std::cout;
  }
  if (os_.empty()) {
    os_.push_back(nullptr);
  }
  auto *ofs = new std::ofstream(filename, mode);
  if (!ofs->is_open()) {
    throw IOError("Cannot open file for writing");
  } else {
    os_.push_back(ofs);
  }
  return *ofs;
}

/**
 *
 */
std::ostream &IO::output(const std::string &filename, std::ios::openmode mode) {
  return output(filename.c_str(), mode);
}

}
