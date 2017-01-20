###################################################
# Define Project name
###################################################
PRJ_NAME=lib
SVN_REV=0
DEBUG=0

OBJDIR=obj
SRCDIR=src
INCDIR=src
LSTDIR=lst

LIBS=

###################################################
# Check compiling system
#  export MINGW32=1 to perform cross build
###################################################
ifneq (,$(MINGW32))
SYSTEM_MSW=1
SYSTEM_LINUX=0
OUT_FILE=$(PRJ_NAME)_msw.a
PLATFORMDIR=$(SRCDIR)/msw
CROSS_PREFIX=i686-w64-mingw32-
else
SYSTEM_MSW=0
SYSTEM_LINUX=1
OUT_FILE=$(PRJ_NAME)_linux.a
PLATFORMDIR=$(SRCDIR)/linux
CROSS_PREFIX=
endif

###################################################
# Source file list
###################################################
C_SRC=$(notdir $(wildcard $(SRCDIR)/*.c))
CPP_SRC=$(notdir $(wildcard $(SRCDIR)/*.cpp))
A_SRC=$(notdir $(wildcard $(SRCDIR)/*.s))
PLATFORM_SRC=$(notdir $(wildcard $(PLATFORMDIR)/*.cpp))

###################################################
# Gen object file name automatically
###################################################
C_OBJ=$(addprefix $(OBJDIR)/, $(C_SRC:.c=.o))
CPP_OBJ=$(addprefix $(OBJDIR)/, $(CPP_SRC:.cpp=.o))
A_OBJ=$(addprefix $(OBJDIR)/, $(A_SRC:.s=.o))
PLATFORM_OBJ=$(addprefix $(OBJDIR)/, $(PLATFORM_SRC:.cpp=.o))
LIBS_OBJ=$(addprefix -l, $(LIBS))


###################################################
# Tools setup
###################################################
INCLUDES=-I$(INCDIR) -I.

DEFINES=-DSVN_REV=$(SVN_REV) -DDEBUG=$(DEBUG) -DPRJ_NAME=$(PRJ_NAME) \
		-DSYSTEM_MSW=$(SYSTEM_MSW) -DSYSTEM_LINUX=$(SYSTEM_LINUX)


CC=$(CROSS_PREFIX)g++
CFLAGS=-c -O -Wall $(DEFINES) $(INCLUDES)

CPP=$(CROSS_PREFIX)g++
CPPFLAGS=$(CFLAGS)

ASM=$(CROSS_PREFIX)g++
ASFLAGS=$(CFLAGS)

AR=$(CROSS_PREFIX)ar
ARFLAGS=-rc

STRIP=$(CROSS_PREFIX)strip

###################################################
# Link file
###################################################
OBJS=$(A_OBJ) $(C_OBJ) $(CPP_OBJ) $(PLATFORM_OBJ)

$(OUT_FILE) : $(OBJS)
	@echo .
	@echo [$(OUT_FILE)]
	@$(AR) $(ARFLAGS) $@ $(OBJS)

###################################################
###################################################
$(SRCDIR)/%.rc : $(INCDIR)/rc.h
	@touch $(SRCDIR)/$(@F)
	
$(SRCDIR)/%.cpp : $(INCDIR)/%.h
	@touch $(SRCDIR)/$(@F)
	
$(SRCDIR)/%.c : $(INCDIR)/%.h
	@touch $(SRCDIR)/$(@F)

###################################################
# Compile C++ files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo [$<]
	@$(CPP) $(CPPFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)
	@$(STRIP) -g $@

$(OBJDIR)/%.o : $(PLATFORMDIR)/%.cpp
	@echo [$<]
	@$(CPP) $(CPPFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)
	@$(STRIP) -g $@

###################################################
# Compile C files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo [$<]
	@$(CC) $(CFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)
	@$(STRIP) -g $@

###################################################
# Assembly ASM files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.s
	@echo [$<]
	@$(ASM) $(ASFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)
	@$(STRIP) -g $@

###################################################
# Clear all object file and output file
###################################################
.PHONY : clean
clean :
	@echo Delete all object file.
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
	@if [ ! -d $(LSTDIR) ]; then mkdir $(LSTDIR); fi
	@rm -f $(OBJDIR)/*
	@rm -f $(LSTDIR)/*
	@rm -f $(PRJ_NAME)_msw.a
	@rm -f $(PRJ_NAME)_linux.a
	@rm -f $(PRJ_NAME).exe
