CXX = c++ -std=c++11
CXXFLAGS = -O3 -Iinclude -Ilib/arkit/include

BINDIR = .
LIBDIR = lib/arkit
SUBDIRS = $(LIBDIR)

BIN = $(BINDIR)/epsi
LIB = libarkit.a
SRC = src/Epsi.cpp

all: $(BIN)

$(BIN): $(SRC) $(LIBDIR)/$(LIB)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LIBDIR)/$(LIB)

$(LIBDIR)/$(LIB): $(LIBDIR)
	(cd $(LIBDIR); $(MAKE))

demo: $(BIN)
	$(BIN) -v -i data/demo.txt -o out-demo.txt -s 2

test: $(BIN)
	$(BIN) -v -i data/test.txt -o out-test.txt -s 20 --delimiter=0

clean-lib:
	(cd $(LIBDIR); $(MAKE) clean)

clean-out:
	rm -f out-*.txt

clean: clean-lib clean-out
	rm -f $(BIN)

