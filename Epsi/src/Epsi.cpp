/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Epsi.cpp 641 2020-08-16 15:23:50Z li $
 */

#include "Epsi.h"
#include "Epsi/Version.h"

typedef item_type Item;
typedef large_item_type LargeItem;
typedef small_item_type SmallItem;
typedef tiny_item_type TinyItem;
typedef size_type Size;
typedef large_size_type LargeSize;
typedef small_size_type SmallSize;
typedef tiny_size_type TinySize;

int main(int argc, char *argv[]) {
  char data = 0;
  char item = 0;
  Options opt("Epsi");
  opt.add("-C", "output patterns with class label");
  opt.add("-D", "output patterns without direct pattern output");
  opt.add("-I", "output patterns with ID (without -S)").excludes("-S");
  opt.add("-R", "output patterns with raw data format");
  opt.add("-S", "output patterns without support values (without -I)").excludes("-I");
  opt.add("-U", "output patterns with unsorted itemsets");
  opt.add("-c", "input data with class label");
  opt.add("-i", "FILE", "", "input file");
  opt.add("-l", "treat itemsets as ordered lists");
  opt.add("-o", "FILE", "", "output file");
  opt.add("-s", "NUMBER", "", "minimum support", Options::REQUIRED);
  opt.add("-w", "input weighted data");
  opt.add("--data", "SIZE", "", "data set size");
  opt.add("--delimiter", "CHAR", ".", "set itemset delimiter to CHAR");
  opt.add("--item", "SIZE", "", "item ID size, or 'auto' for auto-detection");
  opt.add("--max-distance", "NUMBER", "0", "set maximum distance to NUMBER");
  opt.add("--max-items", "NUMBER", "0", "set maximum items to NUMBER");
  opt.add("--max-itemsets", "NUMBER", "0", "set maximum itemsets to NUMBER");
  opt.add("--min-distance", "NUMBER", "0", "set minimum distance to NUMBER");
  opt.add("--min-items", "NUMBER", "0", "set minimum items to NUMBER");
  opt.add("--min-itemsets", "NUMBER", "0", "set minimum itemsets to NUMBER");
  opt.add("--without-pruning", "do not prune non-frequent items");
  opt.add("--without-weight", "ignore weighting（if -w is set)").includes("-w");
  opt.add("--always-output-real-support").hide();
  opt.add("--dump-sdb").hide();
  opt.alias("--help", "-h");
  opt.alias("--verbose", "-v");
  opt.alias("--version", "-V");
  opt.notice("    SIZE includes the following options\n"
             "    - large: 64-bit unsigned integer\n"
             "    - small: 16-bit unsigned integer\n"
             "    - tiny: 8-bit unsigned integer\n"
             "    - default: 32-bit unsigned integer\n");
  opt.syntax("[-i FILE] [-o FILE] -s NUMBER [options] | --help");
  opt.version(Epsi::V::version, Epsi::V::revision, Epsi::V::update, Epsi::V::build).set(
      "Epsi: Extended PrefixSpan Implementation").copyright(
      "Copyright (c) 2020 Dominique Li <dominique.li@univ-tours.fr>\n"
      "PrefixSpan is an algorithm designed by Pei et al. (ICDE 2001)");
  if (!opt(argc, argv)) {
    exit(0);
  }

  // Verbose mode and logging
  Log log(opt);

  // Default configuration
  io::format::text = true;

  // -C
  // output patterns with class label
  if (opt.boolean("-C")) {
    io::output::label = true;
    log(0) << "[-C ON] output patterns with class label.";
  } else {
    io::output::label = false;
    log(0) << "[-C OFF] output patterns without class label.";
  }

  // -D
  // output patterns without direct pattern output
  if (opt.boolean("-D")) {
    Setup::XDPO = (ostream *) nullptr;
    log(0) << "[-D ON] output patterns without direct pattern output.";
  } else {
    Setup::XDPO = (ostream *) 1;
    log(0) << "[-D OFF] output patterns with direct pattern output.";
  }

  // -I
  // output patterns with ID
  if (opt.boolean("-I")) {
    Setup::ShowID = true;
    io::output::support = true; // Set -S to true.
    log(0) << "[-I ON] output patterns with ID.";
  } else {
    log(0) << "[-I OFF] output patterns without ID.";
  }

  // -R
  // output patterns with raw data format
  if (opt.boolean("-R")) {
    io::output::raw = true;
    log(0) << "[-R ON] output patterns with raw data format.";
  } else {
    io::output::raw = false;
    log(0) << "[-R OFF] output patterns without raw data format.";
  }

  // -S
  // output patterns without support values
  if (opt.boolean("-S")) {
    io::output::support = false;
    log(0) << "[-S ON] output patterns without support values.";
  } else {
    io::output::support = true;
    log(0) << "[-S OFF] output patterns with support values.";
  }

  // -U
  // output patterns with unsorted itemsets
  if (opt.boolean("-U")) {
    io::output::sort = false;
    log(0) << "[-U ON] output patterns with unsorted itemsets.";
  } else {
    io::output::sort = true;
    log(0) << "[-U OFF] output patterns without unsorted itemsets.";
  }

  // -c
  // input data with class label
  if (opt.boolean("-c")) {
    io::input::label = true;
    log(0) << "[-c ON] input data with class label.";
  } else {
    io::input::label = false;
    log(0) << "[-c OFF] input data without class label.";
  }

  // -l
  // treat itemsets as ordered lists
  if (opt.boolean("-l")) {
    options::itemset::list = true;
    io::output::sort = false;
    log(0) << "[-l ON] treat itemsets as ordered lists.";
  } else {
    options::itemset::list = false;
    log(0) << "[-l OFF] do not treat itemsets as ordered lists.";
  }

  // -s NUMBER
  // minimum support
  Setup::Support = opt.number("-s");
  if (Setup::Support >= 0) {
    log(0) << "[-s] minimum support set to " << Setup::Support << ".";
  } else {
    opt.usage("invalid minimum support value");
  }

  // -w
  // input weighted data
  if (opt.boolean("-w")) {
    io::input::weight = true;
    Setup::Weighted = true;
    io::output::weight = io::output::support;
    io::output::support = opt.boolean("--always-output-real-support");
    log(0) << "[-w ON] input weighted data.";
  } else {
    io::input::weight = false;
    log(0) << "[-w OFF] do not input weighted data.";
  }

  // --data
  // 'large' (64-bit) or 'small' (16-bit) data set
  if (opt["--data"] == "large") {
    data = 'L';
  } else if (opt["--data"] == "small") {
    data = 'S';
  } else if (opt["--data"] == "tiny") {
    data = 'T';
  } else if (!opt["--data"].empty()) {
    opt.usage("unknown data set size");
  }
  if (opt["--data"].empty()) {
    log(0) << "Data: default 32-bit size data set";
  } else {
    log(0) << "Data: " << opt["--data"];
  }

  // --delimiter=CHAR
  // set itemset delimiter to CHAR
  string delim = opt["--delimiter"];
  if (delim.length()) {
    io::format::delim::text = delim.c_str();
  } else {
    io::format::delim::text = ".";
  }
  log(0) << "Delimiter: " << io::format::delim::text;

  // --item=SIZE
  // 'large' (64-bit), 'small' (16-bit), 'tiny' (8-bit), or 'auto' item ID
  if (opt["--item"] == "auto") {
    item = 'A';
  } else if (opt["--item"] == "large") {
    item = 'L';
  } else if (opt["--item"] == "small") {
    item = 'S';
  } else if (opt["--item"] == "tiny") {
    item = 'T';
  } else if (!opt["--item"].empty()) {
    opt.usage("unknown item size");
  }
  if (opt["--item"].empty()) {
    log(0) << "Item: default 32-bit item ID";
  } else {
    log(0) << "Item: " << opt["--item"];
  }

  // --max-distance=NUMBER
  // set maximum distance to NUMBER
  auto MaxDistance = (int) opt.number("--max-distance");
  if (MaxDistance < 0) {
    opt.usage("invalid maximum distance value");
  } else {
    Setup::MaxDistance = (size_t) MaxDistance;
  }
  if (MaxDistance) {
    log(0) << "Maximum distance: " << MaxDistance;
  }

  // --max-items=NUMBER
  // set maximum number of items to NUMBER
  auto MaxLength = (int) opt.number("--max-items");
  if (MaxLength < 0) {
    opt.usage("invalid maximum number of items");
  } else {
    Setup::MaxLength = (size_t) MaxLength;
  }
  if (MaxLength) {
    log(0) << "Maximum items: " << MaxLength;
  }

  // --max-itemsets=NUMBER
  // set maximum number of itemsets to NUMBER
  auto MaxOrder = (int) opt.number("--max-itemsets");
  if (MaxOrder < 0) {
    opt.usage("invalid maximum number of itemsets");
  } else {
    Setup::MaxOrder = (size_t) MaxOrder;
  }
  if (MaxOrder) {
    log(0) << "Maximum order: " << MaxOrder;
  }

  // --min-distance=NUMBER
  // set minimum distance to NUMBER
  auto MinDistance = (int) opt.number("--min-distance");
  if (MinDistance < 0) {
    opt.usage("invalid minimum distance value");
  } else {
    Setup::MinDistance = (size_t) MinDistance;
  }
  if (MinDistance) {
    log(0) << "Minimum distance: " << MinDistance;
  }

  // --min-items=NUMBER
  // set minimum number of items to NUMBER
  auto MinLength = (int) opt.number("--min-items");
  if (MinLength < 0) {
    opt.usage("invalid minimum number of items");
  } else {
    Setup::MinLength = (size_t) MinLength;
  }
  if (MinLength) {
    log(0) << "Minimum length: " << MinLength;
  }

  // --min-itemsets=NUMBER
  // set minimum number of itemsets to NUMBER
  auto MinOrder = (int) opt.number("--min-itemsets");
  if (MinOrder < 0) {
    opt.usage("invalid minimum number of itemsets");
  } else {
    Setup::MinOrder = (size_t) MinOrder;
  }
  if (MinOrder) {
    log(0) << "Minimum order: " << MinOrder;
  }

  // Verification of maximum/minimum values
  if (MaxDistance && MaxDistance < MinDistance) {
    opt.usage("invalid maximum/minimum distance values");
  }
  if (MaxLength && MaxLength < MinLength) {
    opt.usage("invalid maximum/minimum number of items");
  }
  if (MaxOrder && MaxOrder < MinOrder) {
    opt.usage("invalid maximum/minimum number of itemsets");
  }

  // --without-pruning
  // do not prune non-frequent items
  if (opt.boolean("--without-pruning")) {
    Setup::PruneItems = false;
    log(0) << "[--without-pruning YES] do not prune non-frequent items.";
  } else {
    Setup::PruneItems = true;
    log(0) << "[--without-pruning NO] prune non-frequent items.";
  }

  // --without-weight
  // ignore weighting（if -w is set)
  if (opt.boolean("--without-weight") && Setup::Weighted) {
    Setup::Weighted = false;
    log(0) << "[--without-weight YES] ignore weighting.";
  } else {
    log(0) << "[--without-weight NO] do not ignore weighting.";
  }

  if (data == 'L') {
    log(0) << "Large (64-bit) data set enabled.";
    switch (item) {
    case 'L':
      log(0) << "Large (64-bit) item ID enabled.";
      return MethodEpsi<LargeItem, LargeSize>::exec(opt, log);
    case 'S':
      log(0) << "Small (16-bit) item ID enabled.";
      return MethodEpsi<SmallItem, LargeSize>::exec(opt, log);
    case 'T':
      log(0) << "Tiny (8-bit) item ID enabled.";
      return MethodEpsi<TinyItem, LargeSize>::exec(opt, log);
    default:
      log(0) << "Default (32-bit) item ID enabled.";
      return MethodEpsi<Item, LargeSize>::exec(opt, log);
    }
  } else if (data == 'S') {
    log(0) << "Small (16-bit) data set enabled.";
    switch (item) {
    case 'L':
      log(0) << "Large (64-bit) item ID enabled.";
      return MethodEpsi<LargeItem, SmallSize>::exec(opt, log);
    case 'S':
      log(0) << "Small (16-bit) item ID enabled.";
      return MethodEpsi<SmallItem, SmallSize>::exec(opt, log);
    case 'T':
      log(0) << "Tiny (8-bit) item ID enabled.";
      return MethodEpsi<TinyItem, SmallSize>::exec(opt, log);
    default:
      log(0) << "Default (32-bit) item ID enabled.";
      return MethodEpsi<Item, SmallSize>::exec(opt, log);
    }
  } else if (data == 'T') {
    log(0) << "Tiny (8-bit) data set enabled.";
    switch (item) {
    case 'L':
      log(0) << "Large (64-bit) item ID enabled.";
      return MethodEpsi<LargeItem, TinySize>::exec(opt, log);
    case 'S':
      log(0) << "Small (16-bit) item ID enabled.";
      return MethodEpsi<SmallItem, TinySize>::exec(opt, log);
    case 'T':
      log(0) << "Tiny (8-bit) item ID enabled.";
      return MethodEpsi<TinyItem, TinySize>::exec(opt, log);
    default:
      log(0) << "Default (32-bit) item ID enabled.";
      return MethodEpsi<Item, TinySize>::exec(opt, log);
    }
  } else {
    log(0) << "Default (32-bit) data set enabled.";
    switch (item) {
    case 'L':
      log(0) << "Large (64-bit) item ID enabled.";
      return MethodEpsi<LargeItem, Size>::exec(opt, log);
    case 'S':
      log(0) << "Small (16-bit) item ID enabled.";
      return MethodEpsi<SmallItem, Size>::exec(opt, log);
    case 'T':
      log(0) << "Tiny (8-bit) item ID enabled.";
      return MethodEpsi<TinyItem, Size>::exec(opt, log);
    default:
      log(0) << "Default (32-bit) item ID enabled.";
      return MethodEpsi<Item, Size>::exec(opt, log);
    }
  }
}
