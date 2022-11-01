CC = gcc
CFLAGS = -static -Wall -Os
LDFLAGS = -static

SRCDIR := src
OBJDIR := obj
LIBDIR := lib

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.c)          
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

mutta-console: $(OBJS) link

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
link:
	@ echo !==== LINKING
	$(CC) $(LDFLAGS) $(OBJS) -o mutta-init

clean:
	@ echo !==== CLEANING
	@ rm -rf $(OBJDIR)/*

setup:
	@ echo !==== CREATING DIRECTORIES
	@ mkdir $(OBJDIR)
