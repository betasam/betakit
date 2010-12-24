#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
# Copyright (c) 2005-2012 Sunil Beta Baskar <betasam@gmail.com>
#

all: libbdata

DATASTRUCT_SRCS = list.c graph.c number.c stack.c queue.c bstring.c
INTERNAL_OBJS   = list.o graph.o number.o stack.o queue.o bstring.o
DATASTRUCT_OBJS = $(shell for f in $(INTERNAL_OBJS); do echo $(TOP_DIR)/$(OBJ_DIR)/$$f; done)

DATASTRUCT_LIB = libbdata.so

$(INTERNAL_OBJS): $(DATASTRUCT_SRCS)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(INCLUDES) -c $(shell $(ECHO) $@ | $(SED) s/\.o/\.c/) -o $(TOP_DIR)/$(OBJ_DIR)/$@

libbdata: $(INTERNAL_OBJS)
	$(CC) $(SHAREDLIB) -Wl,-soname,$(DATASTRUCT_LIB).$(VERSION) \
	 -o $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB).$(VERSION).$(SUBVERSION) $(DATASTRUCT_OBJS)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB).$(VERSION)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB)

clean: 
	@$(RM) -f $(DATASTRUCT_OBJS)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB).$(VERSION).$(SUBVERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB).$(VERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(DATASTRUCT_LIB)

.PHONY: all clean $(DATASTRUCT_OBJS) libbdata
