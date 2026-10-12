# $Id$

CC ?= gcc
AR ?= ar

CFLAGS ?= -g -O2 -fPIC
ARFLAGS ?= ruv

LIBS =

OBJS = \
	src/antoi.o \
	src/charcmp.o \
	src/decode_metar.o \
	src/decode_metar_remark.o \
	src/fracpart.o \
	src/print_decoded_metar.o \
	src/stspack2.o \
	src/stspack3.o

.PHONY: all library test install clean

# Default target
all: metar_test library

library: libmetar.a

# Static library creation
libmetar.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

# Test executable
metar_test: src/metar_test.o libmetar.a
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Source dependencies
src/antoi.o: src/antoi.c src/local.h
	$(CC) $(CFLAGS) -c $< -o $@

src/charcmp.o: src/charcmp.c src/local.h
	$(CC) $(CFLAGS) -c $< -o $@

src/decode_metar.o: src/decode_metar.c src/metar_structs.h
	$(CC) $(CFLAGS) -c $< -o $@

src/decode_metar_remark.o: src/decode_metar_remark.c src/metar_structs.h
	$(CC) $(CFLAGS) -c $< -o $@

src/metar_test.o: src/metar_test.c src/metar_structs.h
	$(CC) $(CFLAGS) -c $< -o $@

src/fracpart.o: src/fracpart.c src/local.h
	$(CC) $(CFLAGS) -c $< -o $@

src/print_decoded_metar.o: src/print_decoded_metar.c src/metar_structs.h
	$(CC) $(CFLAGS) -c $< -o $@

src/stspack2.o: src/stspack2.c src/local.h
	$(CC) $(CFLAGS) -c $< -o $@

src/stspack3.o: src/stspack3.c src/local.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: metar_test
	./metar_test

# Installation
install: library
	cp include/metar.h /usr/local/include/
	chmod 0644 /usr/local/include/metar.h
	cp libmetar.a /usr/local/lib/
	chmod 0644 /usr/local/lib/libmetar.a
	ranlib /usr/local/lib/libmetar.a

# Cleanup
clean:
ifeq ($(OS),Windows_NT)
	del /q src\*.o 2>NUL
	del /q metar_test.exe 2>NUL
	del /q libmetar.a 2>NUL
else
	rm -f libmetar.a src/*.o src/*.a metar_test
endif
