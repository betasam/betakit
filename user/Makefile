#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to build simple userspace apps
#		on linux and unix-like platforms.
# Copyright (c) 2005-2012 Sunil Beta Baskar <betasam@gmail.com>
#
# CHANGELOG
#	30-DEC-2010	make configure works now
#	22-DEC-2010	Added DEBUG option for Makefile
#	17-DEC-2010	Checked and updated for cross-compiling#
#	08-DEC-2010	Fixed make -C issue
#
# example:
#	$ make NOAUTO=1 all
# (or)	$ make ARCH=arm CROSS=arm-linux-gnueabi- all
# (or)	$ make clean
# (or)	$ make -k
# (or)  $ make all
# (or)  $ make DEBUG=1 all
# (or)  $ make all -- 1> build.log 2> build.err
#

#
# Before every CVS commit, please make sure you:
# $ make clean
# $ make mrproper
#

# toolchain and architecture
ARCH  =
CROSS =
ifeq ($(ARCH),"arm")
BINPREFIX = arm-
endif
ifeq ($(ARCH),"mips")
BINPREFIX = mips-
endif
ifeq ($(ARCH),"xscale")
BINPREFIX = xscale-
endif

BINPREFIX = 
CC    = $(CROSS)gcc
AR    = $(CROSS)ar
LD    = $(CROSS)ld
STRIP = $(CROSS)strip

ifeq ($(NOAUTO),1)
# system tools
LN	= ln
DOXYGEN = Doxygen
CTAGS   = etags
MKDIR   = mkdir
RM      = rm
FIND    = find
XARGS   = xargs
ECHO    = echo
SED     = sed
GREP    = grep
else
# system tools from environment
LN      = $(shell which ln)
DOXYGEN = $(shell which doxygen)
CTAGS   = $(shell which etags)
MKDIR   = $(shell which mkdir)
RM      = $(shell which rm)
FIND    = $(shell which find)
XARGS   = $(shell which xargs)
ECHO    = $(shell which echo)
SED     = $(shell which sed)
GREP    = $(shell which grep)
endif

CD      = cd


# directories
TOP_DIR=$(shell pwd)
INCLUDE_DIR = include
SRC_DIR := src
SRC_SUBDIRS := system datastructs ui test
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
DOC_DIR := doc
PDF_DIR := latex
BUILD_DIR := build

# toolchain options
CFLAGS = -O2 -Wall
LDFLAGS = -L$(TOP_DIR)/$(LIB_DIR)
LIBFLAGS = -fPIC
SHAREDLIB = -shared
INCLUDES = -I$(TOP_DIR)/$(INCLUDE_DIR)
DOXYFILE = $(TOP_DIR)/Doxyfile
TAGSFILE = $(TOP_DIR)/TAGS
MAKE_FLAGS := -j1
CTAGS_OPTS = -f $(TAGSFILE) -e -a
PDF_TARGET := pdf
CONFIG_HEADER := bkconfig.h

ifeq ($(DEBUG),1)
CFLAGS = -O0 -g -Wall
endif

# version information
VERSION = 1
SUBVERSION =0.2

# for all subdirectory makefiles
export CC AR INCLUDES INCLUDE_DIR SRC_SUBDIRS OBJ_DIR BIN_DIR TOP_DIR CFLAGS SRC_DIR
export LIB_DIR LIBFLAGS SHAREDLIB VERSION SUBVERSION LN LDFLAGS RM MKDIR FIND
export ECHO BUILD_DIR SED BINPREFIX STRIP

# targets
all: dep $(SRC_SUBDIRS) tags

clean: 
	for D in $(SRC_SUBDIRS); do \
	 $(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/$$D clean; \
	done
	if [ -d $(TOP_DIR)/$(DOC_DIR)/ ]; then \
	 $(RM) -rf $(TOP_DIR)/$(DOC_DIR)/*; \
	 $(RM) -f $(TOP_DIR)/$(BIN_DIR)/*; \
	 $(RM) -f $(TAGSFILE); \
	 $(RM) -f $(TOP_DIR)/tags; \
	fi 

mrproper: clean
	$(RM) -f $(TOP_DIR)/$(INCLUDE_DIR)/$(CONFIG_HEADER)

build-deps:
	if [ ! -x $(TOP_DIR)/$(BUILD_DIR)/configure.pl ]; then \
	echo "Error: Unresolved dependencies"; \
	exit 2; \
	fi
#
# BETA!
# explicitly specifying make -C target=all 
# to avoid shell variables clobbering
# refer: ruleset for target: 'clean'
#
$(SRC_SUBDIRS):
	$(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/$@ all

dep: build-dirs build-deps

configure:
	$(CD) $(TOP_DIR)/$(BUILD_DIR) && ./configure.pl ../root.bconfig

doc:
	if [ -x $(DOXYGEN) ]; then \
	 $(MKDIR) $(TOP_DIR)/$(DOC_DIR) -p; \
	 $(DOXYGEN) $(DOXYFILE); \
	fi
	$(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(DOC_DIR)/$(PDF_DIR) $(PDF_TARGET)

build-dirs:
	@$(MKDIR) -p $(TOP_DIR)/$(OBJ_DIR) $(TOP_DIR)/$(BIN_DIR) $(TOP_DIR)/$(LIB_DIR) 
	@$(MKDIR) -p $(TOP_DIR)/$(DOC_DIR) $(TOP_DIR)/$(DOC_DIR)/$(PDF_DIR)

tags:
	$(FIND) $(TOP_DIR) -name *.[chS]* | $(GREP) -v ~$$ | $(XARGS) -n1 $(CTAGS) $(CTAGS_OPTS)

.PHONY: clean doc tags mrproper build-dirs build-deps configure

.SILENT: $(SRC_SUBDIRS) clean tags build-dirs build-deps configure

#
# end of Top Makefile
#
