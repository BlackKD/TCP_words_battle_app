CC := gcc
CFLAGS := -MD
CFLAGS += -Wall -Werror -Wfatal-errors
CFLAGS += -O0 -ggdb3

SRC := $(shell find -name "*.c")
OBJ := $(SRC:%.c=%.o)
DEP := $(shell find -name "*.d")

TARGET := client

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

-include $(DEP)

.PHONY: clean

clean:
	rm -rf $(OBJ) $(DEP) $(TARGET) 2> /dev/null
