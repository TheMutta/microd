CC = g++
CFLAGS = -static -Wall -Os -fpermissive
LDFLAGS = -static

SRCDIR := src
OBJDIR := obj
LIBDIR := lib

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)          
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

init: $(OBJS) link
	@ echo !==== STRIPPING
	@ strip ./init
	@ echo !==== DONE

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
link:
	@ echo !==== LINKING
	$(CC) $(LDFLAGS) $(OBJS) -o init

clean:
	@ echo !==== CLEANING
	@ rm -rf $(OBJDIR)/*

setup:
	@ echo !==== CREATING DIRECTORIES
	@ mkdir $(OBJDIR)
