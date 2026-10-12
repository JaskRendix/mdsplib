# $Id$

CC ?= gcc
AR ?= ar

CFLAGS ?= -g -O2 -fPIC
CPPFLAGS += -Isrc
ARFLAGS ?= ruv

LIBS =

OBJS = \
	src/antoi.o \
	src/charcmp.o \
	src/decode_metar.o \
	src/decode_metar_remark.o \
	src/remark_quality_flags.o \
	src/fracpart.o \
	src/print_decoded_metar.o \
	src/stspack2.o \
	src/stspack3.o

TESTS = \
	metar_test \
	antoi_test \
	charcmp_test \
	fracpart_test \
	print_decoded_metar_test \
	stspack2_test \
	stspack3_test

TEST_OBJS = $(addprefix tests/,$(addsuffix .o,$(TESTS)))

.PHONY: all library test install clean

all: $(TESTS) library

library: libmetar.a

libmetar.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

# Build all test executables
$(TESTS): %: tests/%.o libmetar.a
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Generic rule for test objects
tests/%.o: tests/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Source dependencies
src/antoi.o: src/antoi.c src/local.h
src/charcmp.o: src/charcmp.c src/local.h
src/decode_metar.o: src/decode_metar.c src/metar_structs.h
src/decode_metar_remark.o: src/decode_metar_remark.c src/metar_structs.h src/remark_quality_flags.h
src/remark_quality_flags.o: src/remark_quality_flags.c src/remark_quality_flags.h src/metar_structs.h
src/fracpart.o: src/fracpart.c src/local.h
src/print_decoded_metar.o: src/print_decoded_metar.c src/metar_structs.h
src/stspack2.o: src/stspack2.c src/local.h
src/stspack3.o: src/stspack3.c src/local.h

# Generic compilation rule for library sources
src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: $(TESTS)
	@for t in $(TESTS); do ./$$t; done

install: library
	cp include/metar.h /usr/local/include/
	chmod 0644 /usr/local/include/metar.h
	cp libmetar.a /usr/local/lib/
	chmod 0644 /usr/local/lib/libmetar.a
	ranlib /usr/local/lib/libmetar.a

clean:
ifeq ($(OS),Windows_NT)
	del /q src\*.o 2>NUL
	del /q tests\*.o 2>NUL
	del /q *.exe 2>NUL
	del /q libmetar.a 2>NUL
else
	rm -f libmetar.a src/*.o tests/*.o $(TESTS)
endif
