#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to build simple userspace apps
#		on linux and unix-like platforms.
# Copyright (c) 2008-2012 Sunil Beta Baskar <betasam@gmail.com>
#
# CHANGELOG
#	06-JAN-2011	experimental jemalloc support
#			will need some extra work
#	04-JAN-2011	oldconfig support
#			help target added
#			script calls, command calls set to variables
#			install-deps now installs dependencies on debian
#                       like systems.
#	03-JAN-2011	Doxyfile.Template helps building Doxyfile
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
# (or)  $ make help
# (or)  $ make DEBUG=1 all
# (or)  $ make all -- 1> build.log 2> build.err
#

#
# Before every CVS commit, please make sure you:
# $ make clean
# $ make mrproper
#

#
# VERSION information
#
VERSION       := 1
SUBVERSION    := 0.3
VERSION_VAR   := CONFIG_BK_VERSION
VERSION_HEADER:= bkversion.h

# toolchain and architecture
ARCH      =
CROSS     =
BINPREFIX = 

ifeq ($(ARCH),"arm")
BINPREFIX = arm-
endif
ifeq ($(ARCH),"mips")
BINPREFIX = mips-
endif
ifeq ($(ARCH),"xscale")
BINPREFIX = xscale-
endif

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
SUDO    = sudo
APTGET  = apt-get
MAKE    = make
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
SUDO    = $(shell which sudo)
APTGET  = $(shell which apt-get)
MAKE    = $(shell which make)
endif

# internal command calls
CD      = cd
ECHO    = echo
EXIT	= exit

# directories
TOP_DIR  = $(shell pwd)
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
DOC_DIR := doc
PDF_DIR := latex
BUILD_DIR   := build
INCLUDE_DIR  = include
SRC_DIR     := src
SRC_SUBDIRS := system datastructs ui test

# toolchain options
CFLAGS    = -O2 -Wall
LDFLAGS   = -L$(TOP_DIR)/$(LIB_DIR)
LIBFLAGS  = -fPIC
SHAREDLIB = -shared
INCLUDES  = -I$(TOP_DIR)/$(INCLUDE_DIR)

# Make, build options
DOXYFILE        = $(TOP_DIR)/Doxyfile
TAGSFILE        = $(TOP_DIR)/TAGS
MAKE_FLAGS     := -j1
CTAGS_OPTS      = -f $(TAGSFILE) -e -a
PDF_TARGET     := pdf
SOURCECODE_MSK := *.[chS]*

# configure files
CONFIG_SCRIPT := configure.pl
CONFIG_INF    := root.bconfig
CONFIG_HEADER := bkconfig.h
CONFIG_FILE   := .bkconfig
PATHFACTOR_PL := pathfactor.pl

ARG_FORCEOPTS := -f
USE_OLDCONFIG := y
MAK_OLDCONFIG := n
RAW_DOXYFILE  := Doxyfile
TPL_DOXYFILE  := Doxyfile.Template

# 
# package dependencies
#
PACKAGE_DEPS  := perl
PACKAGE_DEPS  += gcc
PACKAGE_DEPS  += binutils
PACKAGE_DEPS  += doxygen
PACKAGE_DEPS  += exuberant-ctags
PACKAGE_DEPS  += grep
PACKAGE_DEPS  += sed
PACKAGE_DEPS  += findutils
PACKAGE_DEPS  += libparse-recdescent-perl

ifeq ($(DEBUG),1)
MAKE_FLAGS += DEBUG=1 
CFLAGS = -O0 -g -Wall
endif

# for all subdirectory makefiles
export CC AR INCLUDES INCLUDE_DIR SRC_SUBDIRS OBJ_DIR BIN_DIR TOP_DIR CFLAGS SRC_DIR
export LIB_DIR LIBFLAGS SHAREDLIB VERSION SUBVERSION LN LDFLAGS RM MKDIR FIND
export ECHO BUILD_DIR SED BINPREFIX STRIP CONFIG_FILE

# default rule, targets = all
all: dep $(SRC_SUBDIRS) tags

clean: 
	for D in $(SRC_SUBDIRS); do \
	 $(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/$$D clean; \
	done
	 $(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/system/jemalloc clean
	if [ -d $(TOP_DIR)/$(DOC_DIR)/ ]; then \
	 $(RM) -rf $(TOP_DIR)/$(DOC_DIR)/*; \
	 $(RM) -f $(TOP_DIR)/$(BIN_DIR)/*; \
	 $(RM) -f $(TAGSFILE); \
	 $(RM) -f $(TOP_DIR)/tags; \
	fi 

mrproper: clean
	$(RM) -f $(TOP_DIR)/$(INCLUDE_DIR)/$(CONFIG_HEADER)
	$(RM) -f $(TOP_DIR)/$(CONFIG_FILE)
	$(RM) -f $(TOP_DIR)/$(RAW_DOXYFILE)

install-deps:
	@$(SUDO) $(APTGET) install $(PACKAGE_DEPS)

build-deps:
	if [ ! -x $(TOP_DIR)/$(BUILD_DIR)/$(CONFIG_SCRIPT) ]; then \
	$(ECHO) "Error! cannot find build/$(CONFIG_SCRIPT)"; \
	$(EXIT) 2; \
	fi
	if [ ! -x $(TOP_DIR)/$(BUILD_DIR)/$(PATHFACTOR_PL) ]; then \
	$(ECHO) "Error! cannot find build/$(PATHFACTOR_PL)"; \
	$(EXIT) 3; \
	fi
	if [ ! -f $(TOP_DIR)/$(CONFIG_FILE) ]; then \
	$(ECHO) "Error! Run 'make configure' first."; \
	$(EXIT) 4; \
	fi

#
# BETA!
# explicitly specifying make -C target=all 
# to avoid shell variables clobbering
# refer: ruleset for target: 'clean'
#
$(SRC_SUBDIRS):
	$(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/$@ all

dep: build-dirs build-deps $(RAW_DOXYFILE)

configure:
	@$(CD) $(TOP_DIR)/$(BUILD_DIR) && ./$(CONFIG_SCRIPT) ../$(CONFIG_INF) ../$(INCLUDE_DIR)/$(CONFIG_HEADER) ../$(CONFIG_FILE) \
	$(MAK_OLDCONFIG)

oldconfig: 
	@if [ ! -f $(TOP_DIR)/$(CONFIG_FILE) ]; then \
	$(ECHO) "Error! cannot find $(CONFIG_FILE)"; \
	$(ECHO) "Try 'make configure' instead"; \
	$(EXIT) 2; \
	fi
	@$(CD) $(TOP_DIR)/$(BUILD_DIR) && ./$(CONFIG_SCRIPT) ../$(CONFIG_INF) ../$(INCLUDE_DIR)/$(CONFIG_HEADER) ../$(CONFIG_FILE) \
	$(USE_OLDCONFIG)

$(RAW_DOXYFILE): $(TPL_DOXYFILE)
	$(TOP_DIR)/$(BUILD_DIR)/$(PATHFACTOR_PL) $< $(TOP_DIR) > $@

doc: $(RAW_DOXYFILE)
	@if [ -x $(DOXYGEN) ]; then \
	 $(MKDIR) $(TOP_DIR)/$(DOC_DIR) -p; \
	 $(DOXYGEN) $(DOXYFILE); \
	fi
	$(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(DOC_DIR)/$(PDF_DIR) $(PDF_TARGET)

build-dirs:
	@$(MKDIR) -p $(TOP_DIR)/$(OBJ_DIR) $(TOP_DIR)/$(BIN_DIR) $(TOP_DIR)/$(LIB_DIR) 
	@$(MKDIR) -p $(TOP_DIR)/$(DOC_DIR) $(TOP_DIR)/$(DOC_DIR)/$(PDF_DIR)

TAGS: tags

tags:
	@$(FIND) $(TOP_DIR) -name $(SOURCECODE_MSK) | $(GREP) -v ~$$ | $(XARGS) -n1 $(CTAGS) $(CTAGS_OPTS)

README: help

help:
	@$(ECHO) "usage: make <target>"
	@$(ECHO) "       <target> is one of"
	@$(ECHO) "       all      - build all relevant targets"
	@$(ECHO) "       clean    - clean up build directories"
	@$(ECHO) "       mrproper - clean for package builds"
	@$(ECHO) "       tags     - create EMACS friendly TAGS"
	@$(ECHO) "       TAGS     - same as above"
	@$(ECHO) "       doc      - build documents using doxygen, pdflatex"
	@$(ECHO) "       dep      - dependencies including skeleton paths"
	@$(ECHO) "                  (required for CVS checkouts)"
	@$(ECHO) "       help     - this help text"
	@$(ECHO) "       README   - the same help"
	@$(ECHO) "       configure  configure betakit userland suite"
	@$(ECHO) "       oldconfig  use old .bkconfig for userland suite"
	@$(ECHO)
	@$(ECHO) "       install-deps - for debian/ubuntu/knoppix users"
	@$(ECHO) "                      this does not install latex"
	@$(ECHO) "                      but installs all other package dependencies."
	@$(ECHO) "                      needs 'sudo' and 'apt'"
	@$(ECHO)
	@$(ECHO) "other: make NOAUTO=1"
	@$(ECHO) "                   does use 'which' to find executables for make."
	@$(ECHO)

.PHONY: clean doc tags mrproper build-dirs build-deps configure $(RAW_DOXYFILE) help README TAGS install-deps

.SILENT: $(SRC_SUBDIRS) clean tags build-dirs build-deps configure $(RAW_DOXYFILE) help README TAGS install-deps

#
# end of Top Makefile
#
