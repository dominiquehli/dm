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
 * $Id: version.h 625 2020-08-15 14:38:40Z li $
 */

#ifndef _ARKIT_VERSION_H_
#define _ARKIT_VERSION_H_

#include <iostream>
#include <string>

namespace arkit {

/**
 *
 */
class Version {
public:
  Version();
  Version(const Version &);
  Version(unsigned int, unsigned int, unsigned int);
  Version(unsigned int, unsigned int, unsigned int, const char *);
  std::string copyright() const;
  Version &copyright(const char *);
  std::string full() const;
  std::string number() const;
  Version &set(const char *);
  Version &set(unsigned int, unsigned int, unsigned int);
  Version &set(unsigned int, unsigned int, unsigned int, const char *);
  bool operator==(const Version &) const;
  bool operator!=(const Version &) const;
  bool operator<(const Version &) const;
  bool operator<=(const Version &) const;
  bool operator>(const Version &) const;
  bool operator>=(const Version &) const;
private:
  std::string copyright_;
  std::string date_;
  std::string full_;
  unsigned int version_ = 0;
  unsigned int revision_ = 0;
  unsigned int update_ = 0;
};

}

#endif /* _ARKIT_VERSION_H_ */
