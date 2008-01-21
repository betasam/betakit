#
# Betakit
# Author:	Sunil Beta <betasam@gmail.com>
# Description:	Beta's toolkit to simplify linux userspace apps.
#

all: testsrc

TEST_SRCS = hello.c
TEST_OBJS = $(TOP_DIR)/$(OBJ_DIR)/memory.o
TEST_LIBS = -lbsys -lbdata -lbui

LDFLAGS += -lm

testsrc: $(TEST_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TEST_LIBS) $(INCLUDES)  $< -o $(TOP_DIR)/$(BIN_DIR)/$(shell echo $< | sed s/\.c//)

clean: $(TEST_SRCS)
	@rm -f $(TOP_DIR)/$(BIN_DIR)/$(shell echo $< | sed s/\.c//)

.PHONY: all clean testsrc
