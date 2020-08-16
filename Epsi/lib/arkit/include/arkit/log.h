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
 * $Id: log.h 625 2020-08-15 14:38:40Z li $
 */

#ifndef _ARKIT_LOG_H_
#define _ARKIT_LOG_H_

#include "options.h"
#include "timer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace arkit {

class Log {
public:
  struct LogStream {
    friend class Log;
    LogStream();
    LogStream(const LogStream &);
    explicit LogStream(std::ostream &);
    LogStream &operator()(bool);
    LogStream &operator()(short);
    LogStream &operator()(unsigned short);
    LogStream &operator()(int);
    LogStream &operator()(unsigned int);
    LogStream &operator()(long);
    LogStream &operator()(unsigned long);
    LogStream &operator()(long long);
    LogStream &operator()(unsigned long long);
    LogStream &operator()(float);
    LogStream &operator()(double);
    LogStream &operator()(long double);
    LogStream &operator()(void *);
    LogStream &operator()(const char *);
    LogStream &operator()(const std::string &);
    LogStream &operator+(bool);
    LogStream &operator+(short);
    LogStream &operator+(unsigned short);
    LogStream &operator+(int);
    LogStream &operator+(unsigned int);
    LogStream &operator+(long);
    LogStream &operator+(unsigned long);
    LogStream &operator+(long long);
    LogStream &operator+(unsigned long long);
    LogStream &operator+(float);
    LogStream &operator+(double);
    LogStream &operator+(long double);
    LogStream &operator+(void *);
    LogStream &operator+(const char *);
    LogStream &operator+(const std::string &);
    LogStream &operator<<(bool);
    LogStream &operator<<(short);
    LogStream &operator<<(unsigned short);
    LogStream &operator<<(int);
    LogStream &operator<<(unsigned int);
    LogStream &operator<<(long);
    LogStream &operator<<(unsigned long);
    LogStream &operator<<(long long);
    LogStream &operator<<(unsigned long long);
    LogStream &operator<<(float);
    LogStream &operator<<(double);
    LogStream &operator<<(long double);
    LogStream &operator<<(void *);
    LogStream &operator<<(const char *);
    LogStream &operator<<(const std::string &);
  private:
    std::ostream *osp_ = nullptr;
    std::ostream *ospv_ = nullptr;
    bool skip_ = false;
    bool verbose_ = false;
  };
  Log() = default;
  Log(const Log &);
  /**
   * Create a logger with specified prefix.
   */
  explicit Log(const char *);
  /**
   * Create a logger from command-line options.
   */
  explicit Log(const Options &);
  ~Log();
  void disable();
  void enable();
  void *endl();
  void *ends();
  int level() const;
  void level(int);
  Log &open(const char *);
  const char *prefix();
  void prefix(const char *);
  Log &reset();
  void set(std::ostream &);
  bool timer() const;
  void timer(bool);
  bool verbose() const;
  void verbose(bool);
  void verbose(std::ostream &);
  LogStream &operator()(int);
protected:
  bool active_ = false;
  bool ends_ = false;
  std::string filename_;
  int level_ = 0;
  LogStream out_;
  std::string prefix_;
  std::ofstream stream_;
  bool timer_ = false;
  bool verbose_ = false;
};

}

#endif /* _ARKIT_LOG_H_ */
