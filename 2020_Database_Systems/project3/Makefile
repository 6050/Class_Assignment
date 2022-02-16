.SUFFIXES: .c .o

CC=gcc

SRCDIR=src/
INC=include/
LIBS=lib/

# SRCS:=$(wildcard src/*.c)
# OBJS:=$(SRCS:.c=.o)

# main source file
TARGET_SRC:=$(SRCDIR)main.c
TARGET_OBJ:=$(SRCDIR)main.o

# Include more files if you write another source file.
SRCS_FOR_LIB:= \
	$(SRCDIR)bptree.c \
	$(SRCDIR)file.c \
	$(SRCDIR)db.c \
	$(SRCDIR)find.c \
	$(SRCDIR)insert.c \
	$(SRCDIR)delete.c \
	$(SRCDIR)buffer.c

OBJS_FOR_LIB:=$(SRCS_FOR_LIB:.c=.o)

CFLAGS+= -g -fPIC -I $(INC)

TARGET=main

all: $(TARGET)

$(TARGET): $(TARGET_OBJ) $(LIBS)/libbpt.a
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIBS) -lbpt

%.o:	%.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm -rf $(TARGET) $(TARGET_OBJ) $(OBJS_FOR_LIB) $(LIBS)*

library: $(OBJS_FOR_LIB)
	gcc -shared -Wl,-soname,libbpt.so -o $(LIBS)libbpt.so $(OBJS_FOR_LIB)

$(LIBS)/libbpt.a: $(OBJS_FOR_LIB)
	ar cr $(LIBS)libbpt.a $(OBJS_FOR_LIB)
