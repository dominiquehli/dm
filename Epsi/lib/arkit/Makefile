CXX = c++ -std=c++11
CXXFLAGS = -O3 $(INCLUDES)
INCLUDES = -Iinclude
LIBSTORE = lib
LINKLIBS =
USERLIBS =

LIBRARY = libarkit.a
OBJECTS = \
	src/error.o \
	src/io.o \
	src/log.o \
	src/method.o \
	src/options.o \
	src/timer.o \
	src/version.o \
	src/xstring.o
SOURCES = $(wildcard src/*.cc)

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar -rcs $@ $(OBJECTS)

install: $(LIBRARY)
	mkdir -p $(LIBSTORE)
	install -m 644 $(LIBRARY) $(LIBSTORE)

clean:
	rm -f *.a src/*.o