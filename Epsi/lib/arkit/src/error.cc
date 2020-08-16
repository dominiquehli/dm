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
 * $Id: error.cc 625 2020-08-15 14:38:40Z li $
 */

#include "arkit/error.h"

namespace arkit {

const char *Exception::what() const noexcept {
  return Error.c_str();
}

// CodeError

void CodeError::exit() {
  CodeError e;
}

void CodeError::exit(const char *error) {
  CodeError e(error);
}

void CodeError::exit(const char *key, const char *error) {
  CodeError e(key, error);
}

CodeError::CodeError() {
  Error = "[Code Error] Unknown code error.";
  throw Exception();
}

CodeError::CodeError(const char *error) {
  Error = "[Code Error] ";
  Error += error;
  throw Exception();
}

CodeError::CodeError(const char *key, const char *error) {
  Error = "[Code Error] ";
  Error += key;
  Error += ": ";
  Error += error;
  throw Exception();
}

// ParameterError

void ParameterError::exit() {
  ParameterError e;
}

void ParameterError::exit(const char *error) {
  ParameterError e(error);
}

void ParameterError::exit(const char *key, const char *error) {
  ParameterError e(key, error);
}

ParameterError::ParameterError() {
  Error = "[Parameter Error] Unknown parameter error.";
  throw Exception();
}

ParameterError::ParameterError(const char *error) {
  Error = "[Parameter Error] ";
  Error += error;
  throw Exception();
}

ParameterError::ParameterError(const char *key, const char *error) {
  Error = "[Parameter Error] ";
  Error += key;
  Error += ": ";
  Error += error;
  throw Exception();
}

// RuntimeError

void RuntimeError::exit() {
  RuntimeError e;
}

void RuntimeError::exit(const char *error) {
  RuntimeError e(error);
}

void RuntimeError::exit(const char *key, const char *error) {
  RuntimeError e(key, error);
}

RuntimeError::RuntimeError() {
  Error = "[Runtime Error] Unknown runtime error.";
  throw Exception();
}

RuntimeError::RuntimeError(const char *error) {
  Error = "[Runtime Error] ";
  Error += error;
  throw Exception();
}

RuntimeError::RuntimeError(const char *key, const char *error) {
  Error = "[Runtime Error] ";
  Error += key;
  Error += ": ";
  Error += error;
  throw Exception();
}

}
