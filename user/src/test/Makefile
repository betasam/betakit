#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
#

all: testsrc

TEST_SRCS = hello.c
TEST_LIBS = -lbsys -lbdata -lbui

LDFLAGS += -lm

testsrc: $(TEST_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TEST_LIBS) $(INCLUDES)  $< -o $(TOP_DIR)/$(BIN_DIR)/$(BINPREFIX)$(shell $(ECHO) $< | $(SED) s/\.c//)

clean: $(TEST_SRCS)
	@$(RM) -f $(TOP_DIR)/$(BIN_DIR)/$(shell $(ECHO) $< | sed s/\.c//)

.PHONY: all clean testsrc

# end of Makefile (test subdir)
