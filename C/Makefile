# Helpful macros
TARGET_EXEC ?= run/plane
OBJ_DIR ?= ./obj
SRC_DIRS ?= ./src

# Macros for CC
CC = gcc-8
CFLAGS = -pedantic -Wall -fopenmp
LIBS = -lm -lgsl -lgslcblas -fopenmp

# Directories
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include Macros
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compile the source
$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $@

# Create object files
$(OBJ_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
