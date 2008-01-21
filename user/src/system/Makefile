#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
#

#
# Copyright (c) 2005-2008 Sunil Beta Baskar <betasam@gmail.com>
#

all: libbsys

DHRYSTONE_SRCS := dhry_timers.c dhrystone.c
DHRYSTONE_OBJS := dhry_timers.o dhrystone.o
WHETSTONE_SRCS := dhry_timers.c whetstone.c
WHETSTONE_OBJS := whetstone.o

SYSTEM_SRCS := memory.c 
INTERNAL_OBJS := memory.o 

# Dhrystone Support
SYSTEM_SRCS += $(DHRYSTONE_SRCS)
INTERNAL_OBJS += $(DHRYSTONE_OBJS)

# Whetstone Support
SYSTEM_SRCS += $(WHETSTONE_SRCS)
INTERNAL_OBJS += $(WHETSTONE_OBJS)


SYSTEM_OBJS := $(shell for f in $(INTERNAL_OBJS); do echo $(TOP_DIR)/$(OBJ_DIR)/$$f; done)

SYSTEM_LIB := libbsys.so

$(INTERNAL_OBJS): $(SYSTEM_SRCS)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(INCLUDES) -c $(shell echo $@ | sed "s/\.o/\.c/") -o $(TOP_DIR)/$(OBJ_DIR)/$@


libbsys: $(INTERNAL_OBJS)
	$(CC) $(SHAREDLIB) -Wl,-soname,$(SYSTEM_LIB).$(VERSION) \
	 -o $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION) $(SYSTEM_OBJS)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB)

clean: 
	@rm -f $(SYSTEM_OBJS)
	@rm -f $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION)
	@rm -f $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION)
	@rm -f $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB)

.PHONY: all clean libbsys
