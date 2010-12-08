#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
# Copyright (c) 2005-2012 Sunil Beta Baskar <betasam@gmail.com>
#
# CHANGELOG
#	8-DEC-2010	Fixed make -C issue

# toolchain

CROSS =
CC = $(CROSS)gcc
AR = $(CROSS)ar
LD = $(CROSS)ld


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
endif

# directories
TOP_DIR=$(shell pwd)
INCLUDE_DIR = include
SRC_DIR := src
SRC_SUBDIRS := system datastructs ui test
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
DOC_DIR := doc

# toolchain options
CFLAGS = -O2 -Wall
LDFLAGS = -L$(TOP_DIR)/$(LIB_DIR)
LIBFLAGS = -fPIC
SHAREDLIB = -shared
INCLUDES = -I$(TOP_DIR)/$(INCLUDE_DIR)
DOXYFILE = $(TOP_DIR)/Doxyfile
MAKE_FLAGS := -j1
CTAGS_OPTS = -f $(TOP_DIR)/TAGS -e -a

# version information
VERSION = 1
SUBVERSION =0.2

# for all subdirectory makefiles
export CC AR INCLUDES INCLUDE_DIR SRC_SUBDIRS OBJ_DIR BIN_DIR TOP_DIR CFLAGS SRC_DIR
export LIB_DIR LIBFLAGS SHAREDLIB VERSION SUBVERSION LN LDFLAGS RM MKDIR FIND
export ECHO SED

# targets
all: $(SRC_SUBDIRS)

clean: 
	for D in $(SRC_SUBDIRS); do \
	 $(MAKE) -C $(TOP_DIR)/$(SRC_DIR)/$$D clean; \
	done
	@$(RM) -rf $(TOP_DIR)/$(DOC_DIR)/*
	@$(RM) -f $(TOP_DIR)/TAGS
	@$(RM) -f $(TOP_DIR)/tags

# BETA!
# explicitly specifying make -C target=all 
# to avoid shell variables clobbering
$(SRC_SUBDIRS):
	$(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/$@ all

doc:
	if [ -x $(DOXYGEN) ]; then \
	 $(MKDIR) $(TOP_DIR)/$(DOC_DIR) -p; \
	 $(DOXYGEN) $(DOXYFILE); \
	fi

build-dirs:
	@$(MKDIR) -p $(TOP_DIR)/$(OBJ_DIR) $(TOP_DIR)/$(BIN_DIR) $(TOP_DIR)/$(LIB_DIR) $(TOP_DIR)/$(DOC_DIR)

tags:
	@$(FIND) $(TOP_DIR) -name *.[chS] | $(XARGS) -n1 $(CTAGS) $(CTAGS_OPTS)

.PHONY: clean doc tags

.SILENT: $(SRC_SUBDIRS) clean tags

#
# end of Top Makefile
#
