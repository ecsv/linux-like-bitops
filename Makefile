#!/usr/bin/make -f
# SPDX-License-Identifier: MIT
# -*- makefile -*-
#
# FEC (systematic block code) test with fixed size symbols and fixed size
# generation
#
# SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>

# fec-test build
BINARY_NAME = fec-test

obj-y += fec_decode.o
obj-y += fec_encode.o
obj-y += fec.o

# fec-test flags and options
CFLAGS += -Wall -W -pedantic -std=gnu99 -fno-strict-aliasing -MD -MP
CPPFLAGS +=
LDLIBS +=

# flags relevant to identify bugs faster on Linux systems
CFLAGS += -g3 -fsanitize=address -fsanitize=undefined -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -Wl,--print-gc-sections

# disable verbose output
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	Q_CC = @echo '   ' CC $@;
	Q_LD = @echo '   ' LD $@;
	export Q_CC
	export Q_LD
endif
endif

# standard build tools
CC ?= gcc
RM ?= rm -f
COMPILE.c = $(Q_CC)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH)

# standard install paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# default target
all: $(BINARY_NAME)

# standard build rules
.SUFFIXES: .o .c
.c.o:
	$(COMPILE.c) -o $@ $<

$(BINARY_NAME): $(obj-y)
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	$(RM) $(BINARY_NAME) $(obj-y) $(obj-n) $(DEP)

# load dependencies
DEP = $(obj-y:.o=.d) $(obj-n:.o=.d)
-include $(DEP)

.PHONY: all clean
