//
// Created by Dominique Li on 2019-02-01.
//

#ifndef _SDTL_STREAM_UTIL_CC_
#define _SDTL_STREAM_UTIL_CC_

namespace sdtl {

////////////////////////////////////////////////////////////////////////////////
//
// IO utility flags.
//
////////////////////////////////////////////////////////////////////////////////

namespace io {
namespace type {
typedef dictionary<large_item_type> DICO;
}
}

namespace io {
namespace format {
namespace delim {
static char attribute = ':'; // To delimit attribute name and attribute value.
static char extra = '='; // To delimit pattern and its support value.
static int item = 0; // To delimit itemized itemsets.
static const char *text = "."; // To delimit text itemsets.
} // delim
static bool text = true; // Enable text data input/output (built-in dictionary).
} // format
namespace input {
static bool label = false; // Read label value from database.
static bool quality = false; // Read quality value from database.
static bool support = false; // Read support value from database.
static bool weight = false; // Read weight value from database.
} // input
namespace output {
static bool clean = false; // Output without any extended information.
static bool label = false; // Output label value from database.
static bool quality = false; // Output quality value from database.
static bool raw = false; // Enable raw data output.
static bool simple = false; // Output attributes in simple format.
static bool sort = true; // Sort items in output itemsets (with text mode).
static bool support = false; // Output support value from database.
static bool weight = false; // Output weight value from database.
} // output
static io::type::DICO *dico = nullptr; // Online dictionary.
static io::type::DICO *dict = nullptr; // Online dictionary backup.
static bool line = false; // Enable line input/output for sequences.
}

////////////////////////////////////////////////////////////////////////////////
//
// Exceptions.
//
////////////////////////////////////////////////////////////////////////////////

struct dictionary_error : public std::runtime_error {
  dictionary_error() : std::runtime_error("dictionary error") {
  }
  const char *what() const noexcept override {
    return "dictionary error";
  }
};

struct format_error : public std::runtime_error {
  format_error() : std::runtime_error("input format error") {
  }
  const char *what() const noexcept override {
    return "input format error";
  }
};

struct stream_error : public std::runtime_error {
  stream_error() : std::runtime_error("input stream error") {
  }
  const char *what() const noexcept override {
    return "input stream error";
  }
};

}

#endif /* _SDTL_STREAM_UTIL_CC_ */
