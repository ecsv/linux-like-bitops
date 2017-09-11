#!/usr/bin/make -f
# -*- makefile -*-
#
# Minimal Linux-like bit manipulation helper functions test
#
# Copyright (c) 2012-2017, Sven Eckelmann <sven@narfation.org>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

TESTS_CXX_COMPATIBLE = \
 bits_per_byte \
 bits_per_long \
 bitops_div_ceil \
 bitops_ffs \
 bitops_ffz \
 hweight_long \
 hweight8 \
 hweight16 \
 hweight32 \
 hweight64 \
 bit \
 genmask \
 declare_bitmap \
 bitmap_zero \
 bitmap_first_word_mask \
 bitmap_last_word_mask \
 bitmap_fill \
 set_bit \
 clear_bit \
 change_bit \
 test_bit \
 test_and_set_bit \
 test_and_clear_bit \
 test_and_change_bit \
 bitmap_equal \
 bitmap_copy \
 bitmap_empty \
 bitmap_full \
 bitmap_or \
 bitmap_and \
 bitmap_andnot \
 bitmap_xor \
 bitmap_complement \
 bitmap_intersects \
 bitmap_subset \
 bitmap_weight \
 bitmap_set \
 bitmap_clear \
 find_next_bit \
 find_next_zero_bit \
 find_first_bit \
 find_first_zero_bit \
 for_each_set_bit \
 for_each_clear_bit \
 bitops_rol8 \
 bitops_rol16 \
 bitops_rol32 \
 bitops_rol64 \
 bitops_rol_long \
 bitops_ror8 \
 bitops_ror16 \
 bitops_ror32 \
 bitops_ror64 \
 bitops_ror_long \
 bitmap_shift_right \
 bitmap_shift_left \

TESTS_C_ONLY = \

TESTS_ALL = $(TESTS_CXX_COMPATIBLE) $(TESTS_C_ONLY)

# tests flags and options
CFLAGS += -g3 -pedantic -Wall -W -Werror -MD -MP
ifeq ("$(BUILD_CXX)", "1")
	CFLAGS += -std=c++98
	TESTS = $(TESTS_CXX_COMPATIBLE)
	COMPILER_NAME=$(CXX)
else
	CFLAGS += -std=c99
	TESTS += $(TESTS_ALL)
	COMPILER_NAME=$(CC)
endif

# disable verbose output
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	Q_CC = @echo '    $(COMPILER_NAME)' $@;
	Q_LD = @echo '    $(COMPILER_NAME)' $@;
	export Q_CC
	export Q_LD
endif
endif

# standard build tools
CC ?= gcc
RM ?= rm -f
ifeq ("$(BUILD_CXX)", "1")
COMPILE.c = $(Q_CC)$(CXX) -x c++ $(CFLAGS) $(CXXLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CXX) $(CFLAGS) $(CXXLAGS) $(LDFLAGS) $(TARGET_ARCH)
else
COMPILE.c = $(Q_CC)$(CC) -x c $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH)
endif

TESTS_OK = $(TESTS:=.ok)

# default target
all: $(TESTS_OK)

$(TESTS_OK): %.ok: %
	@echo "T:  $(COMPILER_NAME) $(TESTRUN_NAME) $<"
	@$(TESTRUN_WRAPPER) ./$<
	@touch $@

# standard build rules
.SUFFIXES: .o .c
.c.o:
	$(COMPILE.c) -o $@ $<

$(TESTS): %: %.o
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	@$(RM) $(TESTS_ALL) $(DEP) $(TESTS_OK) $(TESTS:=.o) $(TESTS:=.d)

# load dependencies
DEP = $(TESTS:=.d)
-include $(DEP)

.PHONY: all clean
