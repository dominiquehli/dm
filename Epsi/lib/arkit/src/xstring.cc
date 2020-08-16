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
 * $Id: xstring.cc 625 2020-08-15 14:38:40Z li $
 */

#include "arkit/xstring.h"
#include <locale>

namespace arkit {

/**
 * Returns a vector of strings, each of which is a substring of string formed
 * by splitting it on boundaries formed by the delimiter.
 */
std::vector<std::string> explode(const char *delim, const char *str) {
  return explode(delim, std::string(str));
}

/**
 * Returns a vector of strings, each of which is a substring of string formed
 * by splitting it on boundaries formed by the delimiter.
 */
std::vector<std::string> explode(const char *delim, const std::string &str) {
  std::vector<std::string> sub;
  size_t begin = 0;
  size_t end = str.find(delim);
  size_t len = strlen(delim);
  while (end != std::string::npos) {
    sub.push_back(str.substr(begin, end - begin));
    begin = end + len;
    end = str.find(delim, begin);
  }
  sub.push_back(str.substr(begin));
  return sub;
}

/**
 * Convert a string to upper case.
 */
std::string toupper(const char *str) {
  return toupper(std::string(str));
}

/**
 * Convert a string to upper case.
 */
std::string toupper(const std::string &str) {
  std::string upper;
  std::locale loc;
  for (char i : str) {
    upper += std::toupper(i, loc);
  }
  return upper;
}

}
