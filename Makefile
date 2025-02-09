
# Compiler
CC = clang

DEBUG ?= 1

# Compiler flags
CFLAGS = -Wall -std=c11

ifeq ($(DEBUG), 1)
CFLAGS += -ggdb -DDEBUG
$(info INFO: DEBUG MODE ON)
else
CFLAGS += -O3
$(info INFO: DEBUG MODE OFF)
endif

C_SRCS = $(wildcard ./src/*.c) \
	 $(wildcard ./src/**/*.c)

BUILD_DIR = build

C_OBJS = $(C_SRCS:.c=.o)

INCLUDES = -I/usr/include -I. -I./src
# LIB_DIRS = -L/usr/lib

LD_FLAGS = $(LIB_DIRS)

CFLAGS += $(INCLUDES) $(LD_FLAGS)

TARGET = main


##---------------
## BUILD RULES
##---------------

all: $(TARGET)

$(TARGET): $(C_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm $(TARGET)
	rm $(C_OBJS)
