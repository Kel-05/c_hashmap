#
# Compiler flags
#
CC = gcc
#LIBS =
CFLAGS = -Wall -Wextra

#
# Project files
#
SRCS = main.c hashmap.c
OBJS = $(SRCS:.c=.o)
EXE = hashmap

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = out
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O2 -DNDEBUG

.PHONY: all clean debug prep release remake run

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)
	gdb --tui $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(LIBS) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(LIBS) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

run:
	./$(RELEXE) $(arg)

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -rf $(RELDIR) $(DBGDIR)
