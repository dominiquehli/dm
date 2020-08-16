/*-
 * Copyright (c) 2007-2020 Dominique Li <dominique.li@univ-tours.fr>
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
 * $Id: options.cc 639 2020-08-15 23:45:26Z li $
 */

#include "arkit/error.h"
#include "arkit/options.h"
#include "arkit/xstring.h"
#include <iomanip>
#include <sstream>

namespace arkit {

size_t Options::width_ = 0;

/**
 * Output option description information.
 */
std::ostream &operator<<(std::ostream &o, const Options::Option &option) {
  if (!option.name_.length()) {
    return o;
  }
  o << option.name_;
  if (option.type_.length()) {
    switch (Options::hyphen_(option.name_.c_str())) {
    case 1:
      o << " ";
      break;
    case 2:
      o << "=";
      break;
    default:
      break;
    }
    o << option.type_;
  }
  o << std::setw(static_cast<int>(Options::setw_(option)));
  o << " " << option.text_;
  return o;
}

/**
 * Default constructor.
 */
Options::Option::Option() : hidden_(false) {
}

/**
 * Copy constructor.
 */
Options::Option::Option(const Options::Option &o)
    : name_(o.name_), excluded_(o.excluded_), hidden_(false), required_(o.required_), text_(o.text_),
      values_(o.values_) {
}

/**
 * Create a switch option, default value is "NULL" (not set).
 */
Options::Option::Option(const char *name) : hidden_(false), name_(name) {
  values_.emplace_back(std::string("NULL"));
}

/**
 * Create an option with its value type and default value.
 */
Options::Option::Option(const char *name, const char *type, const char *value)
    : hidden_(false), name_(name), type_(type) {
  values_.emplace_back(*value && *type ? std::string(value) : "NULL");
}

/**
 * Add a value to the option.
 */
Options::Option &Options::Option::add(const char *value) {
  if (values_.size() == 1 && values_[0] == "NULL") {
    values_.clear();
  }
  values_.emplace_back(std::string(value));
  return *this;
}

/**
 * Add a conflict option.
 */
Options::Option &Options::Option::excludes(const char *option) {
  std::vector<std::string> exp = explode(":", option);
  for (auto &e: exp) {
    excluded_.insert(e);
  }
  return *this;
}

/**
 * Make the option hidden.
 */
Options::Option &Options::Option::hide() {
  hidden_ = true;
  return *this;
}

/**
 * Returns true if the option is hidden.
 */
bool Options::Option::hidden() const {
  return hidden_;
}

/**
 * Add a dependent option.
 */
Options::Option &Options::Option::includes(const char *option) {
  std::vector<std::string> exp = explode(":", option);
  for (auto &e: exp) {
    required_.insert(e);
  }
  return *this;
}

/**
 * Make an option preset.
 */
Options::Option &Options::Option::preset() {
  preset_ = true;
  return *this;
}

/**
 * Set option value after clearing all existing values.
 */
Options::Option &Options::Option::set(const char *value) {
  values_.clear();
  values_.emplace_back(std::string(value));
  return *this;
}

/**
 * Return option size.
 */
size_t Options::Option::size() const {
  if (values_.size() == 1 && values_[0] == "NULL") {
    return 0;
  }
  return values_.size();
}

/**
 * Return option type.
 */
bool Options::Option::type() const {
  return type_.length() > 0;
}

/**
 * Get text width of the option for formatted output.
 */
size_t Options::Option::width() const {
  return name_.length() + type_.length() + (type_.length() ? 1 : 0);
}

/**
 * Get option name.
 */
const char *Options::Option::operator()() const {
  return name_.c_str();
}

/**
 * Set option description.
 */
Options::Option &Options::Option::operator()(const char *text) {
  text_ = text;
  return *this;
}

/**
 * Get an option value as string in reversed order.
 */
std::string Options::Option::operator[](size_t n) const {
  size_t x = values_.size() - n - 1;
  if (!values_.empty() && n < values_.size()) {
    return values_[x] == "NULL" ? std::string("") : values_[x];
  } else {
    return std::string("");
  }
}

/**
 * Set option value.
 */
Options::Option &Options::Option::operator=(const char *value) {
  values_.clear();
  values_.emplace_back(std::string(value));
  return *this;
}

/**
 * Convert option value to boolean.
 */
Options::Option::operator bool() const {
  if (values_.empty()) {
    return false;
  }
  std::string value = toupper(values_.back());
  if (value == "YES" || value == "TRUE") {
    return true;
  } else if (value == "FALSE" || value == "NO" || value == "NULL") {
    return false;
  }
  return false;
}

/**
 * Convert option value to number.
 */
Options::Option::operator double() const {
  if (values_.empty() || values_.back() == "NULL") {
    return 0.0;
  }
  double value = 0.0;
  std::stringstream ss(values_.back());
  ss >> value;
  return value;
}

/**
 * Convert option value to string.
 */
Options::Option::operator std::string() const {
  if (values_.empty()) {
    return std::string();
  }
  std::string str = values_.back();
  return str == "NULL" ? std::string() : str;
}

/**
 * Show help information to an output stream.
 */
std::ostream &operator<<(std::ostream &o, const Options &options) {
  if (options.syntax_.empty()) {
    return o;
  }
  Options::width_ = 0;
  for (auto &oph0: options.oph0_) {
    if (!oph0.hidden() && oph0.width() > Options::width_) {
      Options::width_ = oph0.width();
    }
  }
  std::map<std::string, Options::Option>::const_iterator it;
  for (auto &oph1: options.oph1_) {
    if (!oph1.second.hidden() && oph1.second.width() > Options::width_) {
      Options::width_ = oph1.second.width();
    }
  }
  for (auto &oph2: options.oph2_) {
    if (!oph2.second.hidden() && oph2.second.width() > Options::width_) {
      Options::width_ = oph2.second.width();
    }
  }
  o << "Usage: " << options.name_ << " " << options.syntax_[0] << std::endl;
  for (auto i = 1; i < options.syntax_.size(); ++i) {
    o << "       ";
    o << options.name_ << " " << options.syntax_[i] << std::endl;
  }
  o << "Options:" << std::endl;
  for (size_t i = 1; i < options.oph0_.size(); ++i) {
    if (options.oph0_[i].hidden()) {
      continue;
    }
    o << "    " << options.oph0_[i] << std::endl;
  }
  for (auto &oph1: options.oph1_) {
    if (oph1.second.hidden_ || oph1.second.preset_) {
      continue;
    }
    o << "    " << oph1.second << std::endl;
  }
  for (auto &oph2: options.oph2_) {
    if (oph2.second.hidden_ || oph2.second.preset_) {
      continue;
    }
    o << "    " << oph2.second << std::endl;
  }
  o << "Help:" << std::endl;
  for (auto &oph1: options.oph1_) {
    if (oph1.second.preset_) {
      o << "    " << oph1.second << std::endl;
    }
  }
  for (auto &oph2: options.oph2_) {
    if (oph2.second.preset_) {
      o << "    " << oph2.second << std::endl;
    }
  }
  o << "Notice:" << std::endl;
  o << options.notice_;
  return o;
}

/**
 * Return the number of hyphens.
 */
char Options::hyphen_(const char *arg) {
  if (arg[0] == '-') {
    if (strlen(arg) == 1) {
      ParameterError::exit("-", "invalid option");
    }
    if (arg[1] != '-') {
      return 1;
    }
    if (arg[1] == '-') {
      if (strlen(arg) == 2) {
        ParameterError::exit("--", "invalid option");
      }
      return 2;
    }
  }
  return 0;
}

/**
 *
 */
size_t Options::setw_(const Option &op) {
  return width_ + 3 - op.width();
}

/**
 * Default constructor.
 */
Options::Options() : error(error_), parsed_(false) {
  Option argv0("argv[0]");
  oph0_.emplace_back(argv0);
  this->required_[0] = std::set<std::string>();
  this->required_[1] = std::set<std::string>();
  default_();
}

/**
 * Create options with application name.
 */
Options::Options(const char *name) : error(error_), name_(name), parsed_(false) {
  Option argv0("argv[0]");
  oph0_.emplace_back(argv0);
  this->required_[0] = std::set<std::string>();
  this->required_[1] = std::set<std::string>();
  default_();
}

/**
 * Destructor.
 */
Options::~Options() = default;

/**
 * Add a switch option.
 * ("Option")
 */
Options::Option &Options::add(const char *name, bool dependence) {
  if (dependence) {
    required_[0].emplace(name);
  }
  Option option(name);
  return add_(option);
}

/**
 * Add a switch option with descriptive text.
 * ("Option", "Description")
 */
Options::Option &Options::add(const char *name, const char *text, bool dependence) {
  if (dependence) {
    required_[0].emplace(name);
  }
  Option option(name);
  option(text);
  return add_(option);
}

/**
 * Add an option with type and default value.
 * ("Option", "Type", "Value")
 */
Options::Option &Options::add(const char *name, const char *type, const char *value, bool dependence) {
  if (dependence) {
    required_[1].emplace(name);
  }
  Option option(name, type, value);
  return add_(option);
}

/**
 * Create an alias option.
 */
Options::Option &Options::alias(const char *from, const char *to) {
  if (oph1_.count(from)) {
    Option option = oph1_[from];
    option.name_ = to;
    option.preset_ = oph1_[from].preset_;
    option.real_ = &oph1_[from];
    oph1_[from].names_.insert(oph1_[from].name_);
    oph1_[from].names_.insert(to);
    return add_(option);
  } else if (oph2_.count(from)) {
    Option option = oph2_[from];
    option.name_ = to;
    option.preset_ = oph2_[from].preset_;
    option.real_ = &oph2_[from];
    oph2_[from].names_.insert(oph2_[from].name_);
    oph2_[from].names_.insert(to);
    return add_(option);
  }
  return empty_;
}

/**
 * Add an option with type, default value and descriptive text.
 * ("Option", "Type", "Value", "Description")
 */
Options::Option &Options::add(const char *name,
                              const char *type,
                              const char *value,
                              const char *text,
                              bool dependence) {
  if (dependence) {
    required_[1].emplace(name);
  }
  Option option(name, type, value);
  option(text);
  return add_(option);
}

/**
 * Get application name.
 */
const char *Options::app() const {
  return name_.c_str();
}

/**
 * Get boolean value of an option.
 */
bool Options::boolean(const char *name) const {
  bool va = false;
  char hy = hyphen_(name);
  std::map<std::string, Option>::const_iterator it;
  switch (hy) {
  case 1:
    it = oph1_.find(name);
    if (it == oph1_.end()) {
      CodeError::exit("Invalid boolean option", name);
    }
    va = it->second.operator bool();
    break;
  case 2:
    it = oph2_.find(name);
    if (it == oph2_.end()) {
      CodeError::exit("Invalid boolean option", name);
    }
    va = it->second.operator bool();
    break;
  default:
    CodeError::exit("Invalid boolean option", name);
    break;
  }
  return va;
}

/**
 * Set notice information.
 */
void Options::notice(const char *text) {
  notice_ = text;
}

/**
 * Get number value of an option.
 */
double Options::number(const char *name) const {
  double va = 0.0;
  char hy = hyphen_(name);
  std::map<std::string, Option>::const_iterator it;
  switch (hy) {
  case 1:
    it = oph1_.find(name);
    if (it == oph1_.end()) {
      CodeError::exit("Invalid number option", name);
    }
    va = it->second.operator double();
    break;
  case 2:
    it = oph2_.find(name);
    if (it == oph2_.end()) {
      CodeError::exit("Invalid number option", name);
    }
    va = it->second.operator double();
    break;
  default:
    CodeError::exit("Invalid number option", name);
    break;
  }
  return va;
}

/**
 * Get string value of an option.
 */
std::string Options::string(const char *name) const {
  std::string va;
  char hy = hyphen_(name);
  std::map<std::string, Option>::const_iterator it;
  switch (hy) {
  case 1:
    it = oph1_.find(name);
    if (it == oph1_.end()) {
      CodeError::exit("Invalid string option", name);
    }
    va = it->second.operator std::string();
    break;
  case 2:
    it = oph2_.find(name);
    if (it == oph2_.end()) {
      CodeError::exit("Invalid string option", name);
    }
    va = it->second.operator std::string();
    break;
  default:
    CodeError::exit("Invalid string option", name);
    break;
  }
  return va;
}

/**
 * Add application command line usage syntax.
 */
void Options::syntax(const char *syntax) {
  syntax_.emplace_back(syntax);
}

/**
 * Trace option stacks.
 */
void Options::trace() const {
  std::cout << "[oph0_] = " << oph0_.size() << std::endl;
  for (auto &oph0: oph0_) {
    std::cout << "    " << oph0() << ": " << oph0[0] << std::endl;
  }
  std::cout << "[oph1_] = " << oph1_.size() << std::endl;
  for (auto &oph1: oph1_) {
    std::cout << "    " << oph1.first << " (" << oph1.second.size() << "):";
    for (int i = 0; i < oph1.second.size(); ++i) {
      std::cout << " " << oph1.second[i];
    }
    std::cout << std::endl;
  }
  std::cout << "[oph2_] = " << oph2_.size() << std::endl;
  for (auto &oph2: oph2_) {
    std::cout << "    " << oph2.first << " (" << oph2.second.size() << "):";
    for (int i = 0; i < oph2.second.size(); ++i) {
      std::cout << " " << oph2.second[i];
    }
    std::cout << std::endl;
  }
}

/**
 * Get usage information.
 */
void Options::usage() const {
  std::cout << *this << std::endl;
  exit(0);
}

/**
 * Get usage information with an error message.
 */
void Options::usage(const char *message) {
  std::cout << message << std::endl;
  std::cout << "Try `" << name_ << " --help' for more information." << std::endl << std::endl;
  exit(0);
}

/**
 * Get version information.
 */
Version &Options::version() {
  return version_;
}

/**
 * Get version information.
 */
const Version &Options::version() const {
  return version_;
}

/**
 * Set version information.
 */
Version &Options::version(unsigned int ve, unsigned int re, unsigned int up) {
  return version_.set(ve, re, up);
}

/**
 * Set version information.
 */
Version &Options::version(unsigned int ve, unsigned int re, unsigned int up, const char *da) {
  return version_.set(ve, re, up, da);
}

/**
 * Return an option.
 */
Options::Option &Options::operator()(const char *name) {
  char hy = hyphen_(name);
  std::map<std::string, Option>::iterator it;
  switch (hy) {
  case 1:
    it = oph1_.find(name);
    if (it == oph1_.end()) {
      CodeError::exit("Invalid option", name);
    }
    return it->second;
    break;
  case 2:
    it = oph2_.find(name);
    if (it == oph2_.end()) {
      CodeError::exit("Invalid option", name);
    }
    return it->second;
    break;
  default:
    CodeError::exit("Invalid option", name);
    break;
  }
  return empty_;
}

/**
 * Parse command-line arguments.
 */
bool Options::operator()(int argc, char **argv) {
  if (argc == 1) {
    this->usage();
  }
  char code = parse_(argc, argv);
  for (auto &&o : this->operator()("--help").names_) {
    if (this->boolean(o.c_str())) {
      std::cout << *this << std::endl;
      return false;
    }
  }
  for (auto &&o : this->operator()("--version").names_) {
    if (this->boolean(o.c_str())) {
      std::cout << version_.full() << std::endl;
      std::cout << "Version " << version_.number() << std::endl;
      std::cout << version_.copyright() << std::endl;
      std::cout << std::endl;
      return false;
    }
  }
  for (auto &&o : this->operator()("--version-number").names_) {
    if (this->boolean(o.c_str())) {
      std::cout << version_.number() << std::endl;
      return false;
    }
  }
  if (code) {
    std::string message;
    switch (code) {
    case 'C': // Common error.
      for (size_t ei = 0; ei < error_.size(); ++ei) {
        message += error_[ei];
        message += "\n";
      }
      break;
    case 'E': // Excluded options found.
      message = error_[0];
      message += ": ";
      message += error_[1];
      message += " and ";
      message += error_[2];
      message += "\n";
      break;
    case 'R': // Required options not found.
      message = "Option ";
      message += error_[2];
      message += " required by ";
      message += error_[1];
      message += "\n";
      break;
    default: // Unknown error, should not happen.
      message = "Unknown error\n";
    }
    this->usage(message.c_str());
  }
  return true;
}

/**
 * Return an option value as a string.
 */
std::string Options::operator[](const char *name) const {
  return this->string(name);
}

/**
 *
 */
std::string Options::operator[](int index) const {
  if (index < oph0_.size()) {
    return oph0_[index][0] == "NULL" ? std::string("") : oph0_[index][0];
  }
  return std::string("");
}

/**
 * Add an option to internal container.
 */
Options::Option &Options::add_(const Option &op) {
  char hy = hyphen_(op());
  std::map<std::string, Option>::const_iterator it;
  switch (hy) {
  case 0:
    oph0_.emplace_back(op);
    return oph0_.back();
  case 1:
    it = oph1_.find(op());
    if (it != oph1_.end()) {
      CodeError::exit("Duplicated option", op());
    }
    oph1_[op()] = op;
    return oph1_[op()];
  case 2:
    it = oph2_.find(op());
    if (it != oph2_.end()) {
      CodeError::exit("Duplicated option", op());
    }
    oph2_[op()] = op;
    return oph2_[op()];
  default:
    CodeError::exit("Invalid option", op());
    break;
  }
  return empty_;
}

/**
 * Clear all options.
 */
void Options::clear_() {
  defined_.clear();
  error_.clear();
  name_ = "";
  oph0_.clear();
  oph1_.clear();
  oph2_.clear();
  parsed_ = false;
  path_ = "";
  syntax_.clear();
}

/**
 * Default preset options.
 */
void Options::default_() {
  this->add("--help", "show help information").preset();
  this->add("--log-file", "FILE", "", "set verbose output file").preset();
  this->add("--log-level", "NUMBER", "0", "set verbose output level").preset();
  this->add("--verbose", "enable verbose output").preset();
  this->add("--version", "show version information").preset();
  this->add("--version-number", "show version number").preset();
}

/**
 * Parse command line arguments.
 */
char Options::parse_(int argc, char **argv) {
  if (argc < 1) {
    RuntimeError::exit("argc < 1");
  }
  char code = 0;
  std::map<std::string, std::string> excluded;
  std::map<std::string, std::set<std::string>> required;
  std::string op, va; // -op va or --op=va
  error_.clear();
  oph0_[0] = argv[0];
  int i = 1;
  int oph0 = 1; // oph0_[0] already presents.
  while (i < argc) {
    int hy = hyphen_(argv[i]);
    switch (hy) {
    case 0:
      if (oph0 < oph0_.size()) {
        oph0_[oph0++] = argv[i];
      } else {
        error_.emplace_back("Undefined argument");
        error_.emplace_back(argv[i]);
        code = 'C';
        ++i;
        break;
      }
      ++i;
      break;
    case 1: { // "-option" switch or "-option value" pair.
      op = argv[i];
      va = "+";
      if (!oph1_.count(op)) {
        error_.emplace_back("Unknown option");
        error_.emplace_back(op);
        code = 'C';
        ++i;
        break;
      }
      ++i;
      Options::Option &o1 = oph1_[op];
      if (o1.type()) { // The option should have a value.
        va = "";
        if (i >= argc) {
          error_.emplace_back("Insufficient arguments");
          error_.emplace_back(op);
          code = 'C';
          ++i;
          break;
        } else {
          if (!hyphen_(argv[i])) {
            va = argv[i++];
          } else {
            error_.emplace_back("Insufficient arguments");
            error_.emplace_back(op);
            code = 'C';
            ++i;
            break;
          }
        }
      } else {
        va = "TRUE";
      }
      if (o1.type()) { // The option should have a value.
        o1.add(va.c_str());
      } else {
        o1 = "TRUE";
      }
      defined_.insert(op);
      if (excluded.count(op)) {
        error_.emplace_back("Incompatible option");
        error_.emplace_back(excluded[op]);
        error_.emplace_back(op);
        code = 'E';
        ++i;
        break;
      }
      for (auto &ex: o1.excluded_) {
        excluded[ex] = o1();
      }
      for (auto &re: required) {
        if (re.second.count(op)) {
          re.second.clear();
        }
      }
      if (required.count(op)) {
        if (!required[op].empty()) {
          for (auto &re: o1.required_) {
            required[op].insert(re);
          }
        }
      } else {
        for (auto &re: o1.required_) {
          required[op].insert(re);
        }
      }
      break;
    }
    case 2: {// "--option" switch or "--option=value" pair.
      std::string s = argv[i];
      op = " ";
      va = "";
      size_t eq = s.find('=');
      if (eq == std::string::npos) { // --option
        op = s.substr(0, s.size());
        va = "";
      } else if (eq > 2) { // --option=value
        op = s.substr(0, eq);
        va = s.substr(eq + 1, s.size());
      } else { // --=
        error_.emplace_back("Invalid option");
        error_.emplace_back(op);
        code = 'C';
        ++i;
        break;
      }
      if (!oph2_.count(op)) {
        error_.emplace_back("Unknown option");
        error_.emplace_back(op);
        code = 'C';
        ++i;
        break;
      }
      ++i;
      Options::Option &o2 = oph2_[op];
      if (o2.type()) { // The option should have a value.
        if (!va.length()) {
          error_.emplace_back("Insufficient arguments");
          error_.emplace_back(op);
          code = 'C';
          ++i;
          break;
        }
        o2.add(va.c_str());
      } else {
        o2 = "TRUE";
      }
      defined_.insert(op);
      if (excluded.count(op)) {
        error_.emplace_back("Incompatible options");
        error_.emplace_back(excluded[op]);
        error_.emplace_back(op);
        code = 'E';
        ++i;
        break;
      }
      for (auto &ex: o2.excluded_) {
        excluded[ex] = o2();
      }
      for (auto &re: required) {
        if (re.second.count(op)) {
          re.second.clear();
        }
      }
      if (required.count(op)) {
        if (!required[op].empty()) {
          for (auto &re: o2.required_) {
            required[op].insert(re);
          }
        }
      } else {
        for (auto &re: o2.required_) {
          required[op].insert(re);
        }
      }
      break;
    }
    default:
      ++i;
    }
  }
  if (!code) {
    for (auto &re: required) {
      if (!re.second.empty()) {
        for (auto &de: defined_) {
          if (re.second.count(de)) {
            re.second.clear();
            break;
          }
        }
      }
    }
    for (auto &re: required) {
      if (!re.second.empty()) {
        error_.emplace_back("Required option");
        error_.emplace_back(re.first);
        auto &&it = re.second.begin();
        std::string second = *it++;
        for (; it != re.second.end(); ++it) {
          second += "/";
          second += *it;
        }
        error_.emplace_back(second);
        code = 'R';
        break;
      }
    }
    if (oph0_.size() - 1 < required_[0].size()) {
      error_.emplace_back("No enough arguments");
      code = 'C';
    }
    for (auto &re: required_[1]) {
      auto &&it1 = oph1_.find(re);
      auto &&it2 = oph2_.find(re);
      if (it1 != oph1_.end()) {
        if (!it1->second.size()) {
          error_.emplace_back("Option");
          error_.emplace_back(re);
          error_.emplace_back("is required");
          code = 'C';
          break;
        }
      } else if (it2 != oph2_.end()) {
        if (!it2->second.size()) {
          error_.emplace_back("Option");
          error_.emplace_back(re);
          error_.emplace_back("is required");
          code = 'C';
          break;
        }
      } else {
        CodeError::exit("Invalid option", re.c_str());
      }
    }
  }
  if (code) {
    return code;
  }
  error_.emplace_back("");
  // Do alias assignment.
  for (auto &&o : oph1_) {
    for (auto &&ox : oph1_) {
      if (ox.first == o.first) {
        continue;
      }
      if (ox.second.names_.count(o.first)) {
        for (auto &&v: o.second.values_) {
          ox.second.values_.push_back(v);
        }
      }
    }
    for (auto &&ox : oph2_) {
      if (ox.first == o.first) {
        continue;
      }
      if (ox.second.names_.count(o.first)) {
        for (auto &&v: o.second.values_) {
          ox.second.values_.push_back(v);
        }
      }
    }
  }
  for (auto &&o : oph2_) {
    for (auto &&ox : oph1_) {
      if (ox.first == o.first) {
        continue;
      }
      if (ox.second.names_.count(o.first)) {
        for (auto &&v: o.second.values_) {
          ox.second.values_.push_back(v);
        }
      }
    }
    for (auto &&ox : oph2_) {
      if (ox.first == o.first) {
        continue;
      }
      if (ox.second.names_.count(o.first)) {
        for (auto &&v: o.second.values_) {
          ox.second.values_.push_back(v);
        }
      }
    }
  }
  return 0;
}

}
