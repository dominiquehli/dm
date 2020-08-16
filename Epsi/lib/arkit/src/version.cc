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
 * $Id: version.cc 625 2020-08-15 14:38:40Z li $
 */

#include "arkit/version.h"
#include <sstream>

namespace arkit {

/**
 *
 */
Version::Version() : date_(""), version_(0), revision_(0), update_(0) {
}

/**
 *
 */
Version::Version(const Version &v) = default;

/**
 *
 */
Version::Version(unsigned int ve, unsigned int re, unsigned int up)
    : date_(""), version_(ve), revision_(re), update_(up) {
}

/**
 *
 */
Version::Version(unsigned int ve, unsigned int re, unsigned int up, const char *da)
    : date_(da), version_(ve), revision_(re), update_(up) {
}

std::string Version::copyright() const {
  return copyright_;
}

Version &Version::copyright(const char *copyright) {
  copyright_ = copyright;
  return *this;
}

std::string Version::full() const {
  return full_;
}

std::string Version::number() const {
  std::string ver;
  std::stringstream vss, rss, uss;
  vss << version_;
  ver = vss.str();
  rss << revision_;
  ver += "." + rss.str();
  if (update_) {
    uss << update_;
    ver += "." + uss.str();
  }
  if (!date_.empty()) {
    ver += "-";
    ver += date_;
  }
  return ver;
}

Version &Version::set(const char *full) {
  full_ = full;
  return *this;
}

Version &Version::set(unsigned int ve, unsigned int re, unsigned int up) {
  version_ = ve;
  revision_ = re;
  update_ = up;
  return *this;
}

Version &Version::set(unsigned int ve, unsigned int re, unsigned int up, const char *da) {
  version_ = ve;
  revision_ = re;
  update_ = up;
  date_ = da;
  return *this;
}

bool Version::operator==(const Version &v) const {
  return version_ == v.version_ && revision_ == v.revision_ && update_ == v.update_;
}

bool Version::operator!=(const Version &v) const {
  return version_ != v.version_ || revision_ != v.revision_ || update_ != v.update_;
}

bool Version::operator<(const Version &v) const {
  if (version_ > v.version_) {
    return false;
  } else if (version_ == v.version_) {
    if (revision_ > v.revision_) {
      return false;
    } else if (revision_ == v.revision_) {
      return update_ < v.update_;
    }
    return true;
  }
  return true;
}

bool Version::operator<=(const Version &v) const {
  return *this == v || *this < v;
}

bool Version::operator>(const Version &v) const {
  if (version_ < v.version_) {
    return false;
  } else if (version_ == v.version_) {
    if (revision_ < v.revision_) {
      return false;
    } else if (revision_ == v.revision_) {
      return update_ > v.update_;
    }
    return true;
  }
  return true;
}

bool Version::operator>=(const Version &v) const {
  return *this == v || *this > v;
}

}
