###################################################
# Define Project name
###################################################
PRJ_NAME=get_voc
SVN_REV=0
DEBUG=0

OBJDIR=obj
SRCDIR=src
INCDIR=src
LSTDIR=lst

LIBDIR=../_lib

###################################################
# Check compiling system
#  export MINGW32=1 to perform cross build
###################################################
ifneq (,$(MINGW32))
SYSTEM_MSW=1
SYSTEM_LINUX=0
OUT_FILE=$(PRJ_NAME)_msw.exe
LIB=$(LIBDIR)/lib_msw.a
CROSS_PREFIX=i686-w64-mingw32-
CROSS_COMPILE=0
else
SYSTEM_MSW=0
SYSTEM_LINUX=1
OUT_FILE=$(PRJ_NAME)_linux
LIB=$(LIBDIR)/lib_linux.a
CROSS_PREFIX=
CROSS_COMPILE=0
endif

###################################################
# Source file list
###################################################
C_SRC=$(notdir $(wildcard $(SRCDIR)/*.c))
CPP_SRC=$(notdir $(wildcard $(SRCDIR)/*.cpp))
A_SRC=$(notdir $(wildcard $(SRCDIR)/*.s))

###################################################
# Gen object file name automatically
###################################################
C_OBJ=$(addprefix $(OBJDIR)/, $(C_SRC:.c=.o))
CPP_OBJ=$(addprefix $(OBJDIR)/, $(CPP_SRC:.cpp=.o))
A_OBJ=$(addprefix $(OBJDIR)/, $(A_SRC:.s=.o))


###################################################
# Tools setup
###################################################
INCLUDES=-I$(INCDIR) -I$(LIBDIR)

DEFINES=-DSVN_REV=$(SVN_REV) -DDEBUG=$(DEBUG) -DCROSS_COMPILE=$(CROSS_COMPILE) -DPRJ_NAME=$(PRJ_NAME) \
		-DSYSTEM_MSW=$(SYSTEM_MSW) -DSYSTEM_LINUX=$(SYSTEM_LINUX)

CC=$(CROSS_PREFIX)g++
CFLAGS=-std=c11 -c -O -Wall $(DEFINES) $(INCLUDES)

CPP=$(CROSS_PREFIX)g++
CPPFLAGS=-std=c++11 -c -O -Wall $(DEFINES) $(INCLUDES)

ASM=$(CROSS_PREFIX)g++
ASFLAGS=$(CFLAGS)

LINK=$(CROSS_PREFIX)g++
LFLAGS=-static

AR=$(CROSS_PREFIX)ar
ARFLAGS=-rc

###################################################
# Link file
###################################################
OBJS=$(A_OBJ) $(C_OBJ) $(CPP_OBJ)

$(OUT_FILE) : $(OBJS)
	@echo .
	@echo [$(OUT_FILE)]
	@$(LINK) $(LFLAGS) -o $(OUT_FILE) $(OBJS) $(LIB)

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

###################################################
# Compile C files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo [$<]
	@$(CC) $(CFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)

###################################################
# Assembly ASM files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.s
	@echo [$<]
	@$(ASM) $(ASFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)

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
	@rm -f $(PRJ_NAME)_msw.exe
	@rm -f $(PRJ_NAME)_linux
