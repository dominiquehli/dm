/*-
 * Epsi (c) 2020 Dominique Li <dominique.li@univ-tours.fr>
 *
 * Epsi (Extended PrefixSpan Implementation) is a C++ implementation of
 * the PrefixSpan-1L algorithm (Pei et al., ICDE 2001) with extensions.
 *
 * Epsi is released under 2-Clause BSD license.
 *
 * $Id: Epsi.h 639 2020-08-15 23:45:26Z li $
 */

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "arkit/error.h"
#include "arkit/log.h"
#include "arkit/method.h"
#include "arkit/options.h"
#include "arkit/version.h"
#include "arkit/xstring.h"
#include "sdtl/measure.h"
#include "sdtl/sdb.h"
#include "sdtl/sequence.h"
#include "sdtl/stream.h"
#include "sdtl/types.h"
#include "Epsi/ItemMap.h"
#include "Epsi/Pattern.h"
#include "Epsi/PatternGrowth.h"
#include "Epsi/PDB.h"
#include "Epsi/Setup.h"
#include "Epsi/Support.h"

using namespace Epsi;
using namespace arkit;
using namespace sdtl;
using namespace std;

template<class ITEM, class SIZE>
struct MethodEpsi : public Method {
  MethodEpsi(const Options &, Log &);
  static int exec(const Options &, Log &);
  int run() override;
};

template<class ITEM, class SIZE>
MethodEpsi<ITEM, SIZE>::MethodEpsi(const Options &opt, Log &log) : Method(opt, log) {
}

template<class ITEM, class SIZE>
int MethodEpsi<ITEM, SIZE>::exec(const Options &opt, Log &log) {
  MethodEpsi<ITEM, SIZE> m = MethodEpsi<ITEM, SIZE>(opt, log);
  return m.run();
}

template<class ITEM, class SIZE>
int MethodEpsi<ITEM, SIZE>::run() {
  try {
    io_.input(options_["-i"]);
    if (options_.boolean("--dump-sdb")) {
      if (options_["--dump-sdb"].length()) {
        io_.output(options_["--dump-sdb"]);
      }
    }
    io_.output(options_["-o"]);
  } catch (...) {
    cerr << "Epsi: cannot open file for input/output" << endl;
    return -1;
  }

  if (Epsi::Setup::XDPO == (ostream *) 1) {
    Epsi::Setup::XDPO = &io_.out();
  }

  sdb<ITEM, SIZE> D;
  Support<ITEM, SIZE> q;
  PatternGrowth<ITEM, SIZE> method(D, q);
  io::dico = &sdtl::dico;

  ITEM BLANC = io::dico->value(EPSI_BLANC);

  if (Setup::PruneItems) {
    Setup::Blanc = (void *) &BLANC;
  }

  log_(0) << "Loading sequence database...";
  timer_.start();
  try {
    io_.in() >> D;
  } catch (exception &e) {
    RuntimeError::exit(e.what());
  }
  timer_.stop();
  log_(0) << "-- " << D.size() << " sequences loaded in " << timer_.runtime() << "s.";

  log_(0) << "Minimum support: " << Setup::Support;
  if (Setup::Support == 0) {
    q.threshold = D.size();
  } else if (Setup::Support > 0 && Setup::Support < 1) {
    q.threshold = (size_t) (D.size() * Setup::Support);
  } else {
    q.threshold = (size_t) Setup::Support;
  }
  log_(0) << "Absolute minimum support: " << q.threshold;

  if (options_.boolean("--dump-sdb")) {
    bool clean = io::output::clean;
    bool raw = io::output::raw;
    io::output::clean = true;
    io::output::raw = true;
    if (options_["--dump-sdb"] != "TRUE") {
      io_.out(1) << D;
    } else {
      io_.error() << D;
    }
    io::output::clean = clean;
    io::output::raw = raw;
  }

  log_(0) << "Generating projected databases... ";
  timer_.start();
  size_t p1L = method.generate1L();
  timer_.stop();
  if (!p1L) {
    log_(0) << "-- Internal error." << log_.endl();
    return 0;
  }
  log_(0) << "-- " << p1L << " level-1 patterns generated in " << timer_.runtime() << "s." << log_.endl();

  timer_.start();
  method.generate();
  timer_.stop();
  log_(0) << "-- " << method.patterns().size() << " patterns generated in " << timer_.runtime() << "s." << log_.endl();

  if (!Setup::XDPO) {
    for (SIZE i = 0; i < method.pdbs().size(); ++i) {
      PDB<ITEM, SIZE> *pdb = method.pdbs()[i];
      if (Setup::MaxOrder && pdb->patt->order > Setup::MaxOrder) {
        continue;
      }
      if (Setup::MinOrder && pdb->patt->order < Setup::MinOrder) {
        continue;
      }
      sequence<ITEM> s;
      s << *pdb;
      io_.out() << s;
      if (Setup::ShowID) {
        io_.out() << " | " << pdb->patt->support << " :";
        if (pdb->imap) {
          for (auto &&pj : *pdb->imap) {
            io_.out() << " " << pj.first;
          }
        }
        io_.out() << " |";
      }
      io_.out() << endl;
    }
  }

  log_(0) << "Program terminated normally.";
  return 0;
}
