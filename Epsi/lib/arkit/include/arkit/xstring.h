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
 * $Id: xstring.h 625 2020-08-15 14:38:40Z li $
 */

#ifndef _ARKIT_XSTRING_H_
#define _ARKIT_XSTRING_H_

#include <string>
#include <vector>

namespace arkit {

/**
 * Returns a vector of strings, each of which is a substring of string formed
 * by splitting it on boundaries formed by the delimiter.
 */
std::vector<std::string> explode(const char *, const char *);

/**
 * Returns a vector of strings, each of which is a substring of string formed
 * by splitting it on boundaries formed by the delimiter.
 */
std::vector<std::string> explode(const char *, const std::string &);

/**
 * Convert a string to upper case.
 */
std::string toupper(const char *);

/**
 * Convert a string to upper case.
 */
std::string toupper(const std::string &);

}

#endif /* _ARKIT_XSTRING_H_ */
