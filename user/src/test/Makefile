#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
#

include $(TOP_DIR)/$(CONFIG_FILE)

all: testsrc

TEST_SRCS = hello.c
TEST_LIBS = -lbsys -lbdata -lbui -lm

ifeq ($(BK_SYS_JEMALLOC),y)
TEST_LIBS += -ljemalloc -lpthread -ldl
endif

LDFLAGS += -lm

testsrc: $(TEST_SRCS)
	$(CC) $(CFLAGS)  $(INCLUDES) $<  $(LDFLAGS) $(TEST_LIBS) -o $(TOP_DIR)/$(BIN_DIR)/$(BINPREFIX)$(shell $(ECHO) $< | $(SED) s/\.c//)

clean: $(TEST_SRCS)
	@$(RM) -f $(TOP_DIR)/$(BIN_DIR)/$(shell $(ECHO) $< | sed s/\.c//)

.PHONY: all clean testsrc

# end of Makefile (test subdir)
