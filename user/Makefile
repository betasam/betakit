#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
#

#
# Copyright (c) 2005-2012 Sunil Beta Baskar <betasam@gmail.com>
#

# toolchain
CROSS =
CC = $(CROSS)gcc
AR = $(CROSS)ar
LD = $(CROSS)ld

# system tools
LN = $(shell which ln)
DOXYGEN = $(shell which doxygen)
CTAGS = $(shell which etags)


# directories

# FIXME!
# what if somebody runs make -C ?
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
MAKEFLAGS = -j1
CTAGS_OPTS = -f $(TOP_DIR)/TAGS -e -a

# version information
VERSION = 1
SUBVERSION = 0.1

# for all subdirectory makefiles
export CC AR INCLUDES INCLUDE_DIR SRC_SUBDIRS OBJ_DIR BIN_DIR TOP_DIR CFLAGS SRC_DIR
export LIB_DIR LIBFLAGS SHAREDLIB VERSION SUBVERSION LN LDFLAGS

# targets
all: $(SRC_SUBDIRS)

clean: 
	for D in $(SRC_SUBDIRS); do \
	 $(MAKE) -C $(TOP_DIR)/$(SRC_DIR)/$$D clean; \
	done
	@rm -rf $(TOP_DIR)/$(DOC_DIR)/*
	@rm -f $(TOP_DIR)/TAGS
	@rm -f $(TOP_DIR)/tags

$(SRC_SUBDIRS):
	$(MAKE) $(MAKEFLAGS) -C $(TOP_DIR)/$(SRC_DIR)/$@

doc:
	if [ -x $(DOXYGEN) ]; then \
	mkdir $(TOP_DIR)/$(DOC_DIR) -p; \
	$(DOXYGEN) $(DOXYFILE); \
	fi

build-dirs:
	@mkdir -p $(TOP_DIR)/$(OBJ_DIR) $(TOP_DIR)/$(BIN_DIR) $(TOP_DIR)/$(LIB_DIR) $(TOP_DIR)/$(DOC_DIR)

tags:
	@find $(TOP_DIR) -name *.[chS] | xargs -n1 $(CTAGS) $(CTAGS_OPTS)

.PHONY: clean doc tags

.SILENT: $(SRC_SUBDIRS) clean tags

#
# end of Top Makefile
#
