#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
# 
# JEMALLOC
# Copyright (c) 2011 Facebook Inc. - Mozilla Public License
#
# CHANGELOG
#
#	06-JAN-2011	Added JEMALLOC Support, (git-source)
#

all: libjemalloc

#
# exclusive to libjemalloc
#
VERSION        := 2
SUBVERSION     := 1.0

JEMALLOC_SRCS  := arena.c base.c chunk.c chunk_dss.c chunk_mmap.c chunk_swap.c ckh.c ctl.c
JEMALLOC_SRCS  += extent.c hash.c huge.c jemalloc.c mb.c mutex.c prof.c rtree.c stats.c tcache.c

JEMALLOC_LIB   := libjemalloc.so

JEMALLOC_OBJS  := arena.o base.o chunk.o chunk_dss.o chunk_mmap.o chunk_swap.o ckh.o ctl.o 
JEMALLOC_OBJS  += extent.o hash.o huge.o jemalloc.o mb.o mutex.o prof.o rtree.o stats.o tcache.o 

CFLAGS         += -D_GNU_SOURCE 

JSYSTEM_OBJS   := $(shell for f in $(JEMALLOC_OBJS); do $(ECHO) $(TOP_DIR)/$(OBJ_DIR)/$$f; done)

$(JEMALLOC_OBJS): $(JEMALLOC_SRCS)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(INCLUDES) -c $(shell $(ECHO) $@ | $(SED) "s/\.o/\.c/") -o $(TOP_DIR)/$(OBJ_DIR)/$@

libjemalloc: $(JEMALLOC_OBJS)
	$(CC) $(SHAREDLIB) -Wl,-soname,$(JEMALLOC_LIB).$(VERSION) \
	 -o $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB).$(VERSION).$(SUBVERSION) $(JSYSTEM_OBJS)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB).$(VERSION)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB)

clean: 
	@$(RM) -f $(JSYSTEM_OBJS)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB).$(VERSION).$(SUBVERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB).$(VERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(JEMALLOC_LIB)

.PHONY: all clean libjemalloc
