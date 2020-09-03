/*-
 * Copyright (c) 2013 Dominique Li <dominique.li@univ-tours.fr>
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
 * $Id: log.cc 645 2020-08-20 14:33:42Z li $
 */

#include <cstring>
#include "arkit/log.h"

namespace arkit {

/**
 *
 */
Log::Log(const Log &log) {
  active_ = log.active_;
  ends_ = log.ends_;
  level_ = log.level_;
  out_ = log.out_;
  filename_ = log.filename_;
  prefix_ = log.prefix_;
  timer_ = log.timer_;
  verbose_ = log.verbose_;
  open(log.filename_.c_str());
}

/**
 *
 */
Log::Log(const char *prefix) : prefix_(prefix) {
}

/**
 *
 */
Log::Log(const Options &opt) {
  filename_ = opt.string("--log-file");
  if (filename_.length()) {
    open(filename_.c_str());
  }
  level_ = (int) opt.number("--log-level");
  prefix_ = opt.app();
  verbose_ = opt.boolean("--verbose");
  this->operator()(0) << opt.app() << " Version " << opt.version().number();
}

/**
 *
 */
Log::~Log() {
  if (active_) {
    *(out_.osp_) << std::endl;
  }
  if (verbose_) {
    *(out_.ospv_) << std::endl;
  }
}

/**
 *
 */
void Log::disable() {
  active_ = false;
}

/**
 *
 */
void Log::enable() {
  active_ = true;
}

/**
 *
 */
void *Log::endl() {
  if (active_) {
    *(out_.osp_) << std::endl;
  }
  if (verbose_) {
    *(out_.ospv_) << std::endl;
  }
  ends_ = true;
  return (void *) nullptr;
}

/**
 *
 */
void *Log::ends() {
  ends_ = true;
  return (void *) nullptr;
}

/**
 *
 */
int Log::level() const {
  return level_;
}

/**
 *
 */
void Log::level(int n) {
  level_ = n;
}

/**
 *
 */
Log &Log::open(const char *file) {
  if (!strlen(file)) {
    out_.osp_ = &stream_;
    filename_ = file;
    return *this;
  }
  if (stream_.is_open()) {
    stream_.close();
  }
  stream_.open(file);
  if (!stream_.good()) {
    std::cerr << "Cannot open file " << file << " for log." << std::endl;
    std::cerr << "Use standard error output." << std::endl;
    out_.osp_ = &std::cerr;
    filename_ = "";
  } else {
    out_.osp_ = &stream_;
    filename_ = file;
  }
  return *this;
}

/**
 *
 */
const char *Log::prefix() {
  return prefix_.c_str();
}

/**
 *
 */
void Log::prefix(const char *s) {
  prefix_ = s;
}

/**
 *
 */
Log &Log::reset() {
  level_ = 0;
  if (stream_.is_open()) {
    stream_.close();
  }
  out_.osp_ = &std::cerr;
  return *this;
}

/**
 *
 */
void Log::set(std::ostream &out) {
  out_.osp_ = &out;
}

/**
 *
 */
bool Log::timer() const {
  return timer_;
}

/**
 *
 */
void Log::timer(bool active) {
  timer_ = active;
}

/**
 *
 */
bool Log::verbose() const {
  return verbose_;
}

/**
 *
 */
void Log::verbose(bool active) {
  verbose_ = active;
}

/**
 *
 */
void Log::verbose(std::ostream &out) {
  out_.ospv_ = &out;
}

/**
 *
 */
Log::LogStream &Log::operator()(int c) {
  if (active_) {
    if (level_ < c) {
      out_.skip_ = true;
    } else {
      out_.skip_ = false;
      if (!ends_) {
        *(out_.osp_) << std::endl;
        if (timer_) {
          if (prefix_.length()) {
            *(out_.osp_) << Timer::now() << " " << prefix_ << ": ";
          } else {
            *(out_.osp_) << Timer::now() << ": ";
          }
        } else {
          if (prefix_.length()) {
            *(out_.osp_) << prefix_ << ": ";
          }
        }
      }
    }
  } else {
    out_.skip_ = true;
  }
  if (verbose_ && level_ >= c) {
    out_.verbose_ = true;
    if (!ends_) {
      *(out_.ospv_) << std::endl;
    }
  } else {
    out_.verbose_ = false;
  }
  ends_ = false;
  return out_;
}

Log::LogStream::LogStream() : osp_(&std::cerr), ospv_(&std::cerr) {
}

Log::LogStream::LogStream(const Log::LogStream &ls) = default;

Log::LogStream::LogStream(std::ostream &out) : osp_(&out), ospv_(&std::cerr) {
}

Log::LogStream &Log::LogStream::operator()(bool x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(short x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(unsigned short x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(int x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(unsigned int x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(unsigned long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(long long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(unsigned long long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(float x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(double x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(long double x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(void *) {
  return *this;
}

Log::LogStream &Log::LogStream::operator()(const char *x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator()(const std::string &x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(bool x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(short x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(unsigned short x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(int x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(unsigned int x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(unsigned long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(long long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(unsigned long long x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(float x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(double x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(long double x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(void *) {
  return *this;
}

Log::LogStream &Log::LogStream::operator+(const char *x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator+(const std::string &x) {
  return this->operator<<(x);
}

Log::LogStream &Log::LogStream::operator<<(bool x) {
  if (verbose_) {
    if (x) {
      *ospv_ << "True";
    } else {
      *ospv_ << "False";
    }
  }
  if (skip_) {
    return *this;
  }
  if (x) {
    *osp_ << "True";
  } else {
    *osp_ << "False";
  }
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(short x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(unsigned short x) {
  if (skip_) {
    return *this;
  }
  if (verbose_) {
    *ospv_ << x;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(int x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(unsigned int x) {
  if (skip_) {
    return *this;
  }
  if (verbose_) {
    *ospv_ << x;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(long x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(unsigned long x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(long long x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(unsigned long long x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(float x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(double x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(long double x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(void *) {
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(const char *x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

Log::LogStream &Log::LogStream::operator<<(const std::string &x) {
  if (verbose_) {
    *ospv_ << x;
  }
  if (skip_) {
    return *this;
  }
  *osp_ << x;
  return *this;
}

}
