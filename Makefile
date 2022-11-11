CC = g++
CFLAGS = -static -Wall -Os -fpermissive
LDFLAGS = -static

SRCDIR := src
INIT_SRCDIR := $(SRCDIR)/init
INITCTL_SRCDIR := $(SRCDIR)/initctl
OBJDIR := obj
INIT_OBJDIR := $(OBJDIR)/init
INITCTL_OBJDIR := $(OBJDIR)/initctl

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

INIT_SRC = $(call rwildcard,$(INIT_SRCDIR),*.cpp)          
INITCTL_SRC = $(call rwildcard,$(INITCTL_SRCDIR),*.cpp)          
INIT_OBJS = $(patsubst $(INIT_SRCDIR)/%.cpp, $(INIT_OBJDIR)/%.o, $(INIT_SRC))
INITCTL_OBJS = $(patsubst $(INITCTL_SRCDIR)/%.cpp, $(INITCTL_OBJDIR)/%.o, $(INITCTL_SRC))
INIT_DIRS = $(wildcard $(INIT_SRCDIR)/*)
INITCTL_DIRS = $(wildcard $(INITCTL_SRCDIR)/*)

all: init initctl

init: $(INIT_OBJS) link_init
	@ echo !==== STRIPPING
	@ strip ./init
	@ echo !==== DONE

initctl: $(INITCTL_OBJS) link_initctl
	@ echo !==== STRIPPING
	@ strip ./initctl
	@ echo !==== DONE


$(INIT_OBJDIR)/%.o: $(INIT_SRCDIR)/%.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(INITCTL_OBJDIR)/%.o: $(INITCTL_SRCDIR)/%.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
link_init:
	@ echo !==== LINKING
	$(CC) $(LDFLAGS) $(INIT_OBJS) -o init

link_initctl:
	@ echo !==== LINKING
	$(CC) $(LDFLAGS) $(INITCTL_OBJS) -o initctl

clean:
	@ echo !==== CLEANING
	@ rm -rf $(INIT_OBJDIR)/* $(INITCTL_OBJDIR)/*

setup:
	@ echo !==== CREATING DIRECTORIES
	@ mkdir -p $(INIT_OBJDIR) $(INITCTL_OBJDIR)
