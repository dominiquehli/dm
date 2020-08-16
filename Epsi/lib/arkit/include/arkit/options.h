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
 * $Id: options.h 639 2020-08-15 23:45:26Z li $
 */

#ifndef _ARKIT_OPTIONS_H_
#define _ARKIT_OPTIONS_H_

#include "version.h"
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace arkit {

class Options {
public:
  struct Option {
    friend class Options;
    friend std::ostream &operator<<(std::ostream &, const Option &);
    friend std::ostream &operator<<(std::ostream &, const Options &);
    Option();
    Option(const Option &);
    explicit Option(const char *);
    Option(const char *, const char *, const char *);
    Option &add(const char *);
    Option &excludes(const char *);
    Option &hide();
    bool hidden() const;
    Option &includes(const char *);
    Option &preset();
    Option &set(const char *);
    size_t size() const;
    bool type() const;
    size_t width() const;
    const char *operator()() const;
    Option &operator()(const char *);
    std::string operator[](size_t) const;
    Option &operator=(const char *);
    explicit operator bool() const;
    explicit operator double() const;
    explicit operator std::string() const;
  private:
    std::set<std::string> excluded_;
    bool hidden_ = false;
    std::string name_;
    std::set<std::string> names_;
    bool preset_ = false;
    Option *real_ = nullptr;
    std::set<std::string> required_;
    std::string text_;
    std::string type_;
    std::vector<std::string> values_;
  };
  friend std::ostream &operator<<(std::ostream &, const Option &);
  friend std::ostream &operator<<(std::ostream &, const Options &);
  static const bool REQUIRED = true;
  Options();
  explicit Options(const char *);
  virtual ~Options();
  Option &add(const char *, bool = false);
  Option &add(const char *, const char *, bool = false);
  Option &add(const char *, const char *, const char *, bool = false);
  Option &add(const char *, const char *, const char *, const char *, bool = false);
  Option &alias(const char *, const char *);
  const char *app() const;
  bool boolean(const char *) const;
  void notice(const char *);
  double number(const char *) const;
  std::string string(const char *) const;
  void syntax(const char *);
  void trace() const;
  void usage() const;
  void usage(const char *);
  Version &version();
  const Version &version() const;
  Version &version(unsigned int, unsigned int, unsigned int);
  Version &version(unsigned int, unsigned int, unsigned int, const char *);
  Option &operator()(const char *);
  bool operator()(int, char **);
  std::string operator[](const char *) const;
  std::string operator[](int) const;
  const std::vector<std::string> &error;
private:
  static char hyphen_(const char *);
  static size_t setw_(const Option &);
  static size_t width_;
  Option &add_(const Option &);
  void clear_();
  void default_();
  char parse_(int, char **);
  std::set<std::string> defined_;
  Option empty_;
  std::vector<std::string> error_;
  std::string name_;
  std::string notice_;
  std::vector<Option> oph0_; // All options without - neither --.
  std::map<std::string, Option> oph1_; // All options starting with -.
  std::map<std::string, Option> oph2_; // All options starting with --.
  bool parsed_;
  std::string path_;
  std::map<char, std::set<std::string>> required_;
  std::vector<std::string> syntax_;
  Version version_;
  std::string zero_;
};

}

#endif /* _ARKIT_OPTIONS_H_ */
