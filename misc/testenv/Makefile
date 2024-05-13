CC := gcc
CFLAGS := -Wall -Wextra -Iinclude
LDFLAGS := -lm

SRCDIR := src
BUILDDIR := build

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
TARGET := myprogram

.PHONY: all clean

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR)

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: $(BUILDDIR)/$(TARGET)

run: debug
	./$(BUILDDIR)/$(TARGET)

valgrind: debug
	valgrind --tool=memcheck --track-origins=yes --show-reachable=yes --leak-check=full ./$(BUILDDIR)/$(TARGET)

