CC := gcc
CFLAGS := -MD
CFLAGS += -Wall -Werror -Wfatal-errors
CFLAGS += -O0 -ggdb3

SRC := $(shell find -name "*.c")
OBJ := $(SRC:%.c=%.o)
DEP := $(shell find -name "*.d")

TARGET := server

$(TARGET): $(OBJ)
	$(CC) $^ -lpthread -o $@

-include $(DEP)

.PHONY: clean

clean:
	rm -rf $(OBJ) $(DEP) $(TARGET) 2> /dev/null
