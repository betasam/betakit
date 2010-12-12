#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
# Copyright (c) 2005-2012 Sunil Beta Baskar <betasam@gmail.com>
#


UI_SRCS = cli.c
INTERNAL_OBJS = cli.o

UI_OBJS = $(shell for f in $(INTERNAL_OBJS); do echo $(TOP_DIR)/$(OBJ_DIR)/$$f; done)

UI_LIB = libbui.so

all: libbui

$(INTERNAL_OBJS): $(UI_SRCS)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(INCLUDES) -c $(shell echo $@ | sed s/\.o/\.c/) -o $(TOP_DIR)/$(OBJ_DIR)/$@


libbui: $(INTERNAL_OBJS)
	@$(CC) $(SHAREDLIB) -Wl,-soname,$(UI_LIB).$(VERSION) \
	 -o $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB).$(VERSION).$(SUBVERSION) $(UI_OBJS)
	@$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB).$(VERSION)
	@$(LN) -snf $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB).$(VERSION).$(SUBVERSION) \
	 $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB)

clean: 
	@$(RM} -f $(UI_OBJS)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB).$(VERSION).$(SUBVERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB).$(VERSION)
	@$(RM) -f $(TOP_DIR)/$(LIB_DIR)/$(UI_LIB)


.PHONY: all clean $(UI_OBJS) libbui
