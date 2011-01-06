#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
# Copyright (c) 2008-2012 Sunil Beta Baskar <betasam@gmail.com>
#
# CHANGELOG
#
#	06-JAN-2011	Added JEMALLOC Support, (git-source)
#

include $(TOP_DIR)/$(CONFIG_FILE)

ifeq ($(BK_SYS_JEMALLOC),y)
JEMALLOC_TARG  := libjemalloc
CFLAGS         += -D_BKIT_JEMALLOC
REF_LIBS       := -L$(TOP_DIR)/$(LIB_DIR) -ljemalloc -lpthread -ldl
JEMALLOC_SRCS  := bjemalloc.c
JEMALLOC_OBJS  := bjemalloc.o
else
JEMALLOC_TARG  :=
JEMALLOC_SRCS  :=
JEMALLOC_OBJS  :=
REF_LIBS       :=
endif

all: libbsys $(JEMALLOC_TARG)

DHRYSTONE_SRCS := dhry_timers.c dhrystone.c
DHRYSTONE_OBJS := dhry_timers.o dhrystone.o

WHETSTONE_SRCS := dhry_timers.c whetstone.c
WHETSTONE_OBJS := whetstone.o

# System and Memory handlers
SYSTEM_SRCS    := memory.c berror.c
INTERNAL_OBJS  := memory.o berror.o

# Dhrystone Support
SYSTEM_SRCS    += $(DHRYSTONE_SRCS)
INTERNAL_OBJS  += $(DHRYSTONE_OBJS)

# Whetstone Support
SYSTEM_SRCS    += $(WHETSTONE_SRCS)
INTERNAL_OBJS  += $(WHETSTONE_OBJS)

# JEMALLOC Support
SYSTEM_SRCS    += $(JEMALLOC_SRCS)
INTERNAL_OBJS  += $(JEMALLOC_OBJS)

SYSTEM_OBJS    := $(shell for f in $(INTERNAL_OBJS); do $(ECHO) $(TOP_DIR)/$(OBJ_DIR)/$$f; done)

SYSTEM_LIB     := libbsys.so

$(INTERNAL_OBJS): $(SYSTEM_SRCS)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(INCLUDES) -c $(shell $(ECHO) $@ | $(SED) "s/\.o/\.c/") -o $(TOP_DIR)/$(OBJ_DIR)/$@

libjemalloc:
	$(MAKE) $(MAKE_FLAGS) -C $(TOP_DIR)/$(SRC_DIR)/system/jemalloc all

libbsys: $(JEMALLOC_TARG) $(INTERNAL_OBJS)
	$(CC) $(SHAREDLIB) $(REF_LIBS) -Wl,-soname,$(SYSTEM_LIB).$(VERSION) \
	 -o $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION) $(SYSTEM_OBJS)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION)
	$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB)


clean: 
	@$(RM) -f $(SYSTEM_OBJS)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION).$(SUBVERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB).$(VERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(SYSTEM_LIB)

.PHONY: all clean libbsys libjemalloc
