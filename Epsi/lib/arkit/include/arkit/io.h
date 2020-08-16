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
 * $Id: io.h 625 2020-08-15 14:38:40Z li $
 */

#ifndef _ARKIT_IO_H_
#define _ARKIT_IO_H_

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace arkit {

class IO {
public:
  struct IOError : public std::runtime_error {
    explicit IOError(const std::string &message) : std::runtime_error(message) {
    }
  };
  IO();
  virtual ~IO();
  void clear();
  void close(std::istream &);
  void close(std::ostream &);
  std::ostream &error(const char *);
  std::ostream &error();
  std::istream &in();
  std::istream &in(size_t);
  std::istream &input(const char *);
  std::istream &input(const std::string &);
  std::ostream &out();
  std::ostream &out(size_t);
  std::ostream &output(const char *);
  std::ostream &output(const std::string &);
  std::ostream &output(const char *, std::ios::openmode);
  std::ostream &output(const std::string &, std::ios::openmode);
private:
  std::ofstream *error_ = nullptr;
  std::vector<std::istream *> is_;
  std::vector<std::ostream *> os_;
};

}

#endif /* _ARKIT_IO_H_ */
