###################################################
# For GNU make
###################################################

###################################################
# Define Project name
###################################################
PRJ_NAME=get_tnh
DEBUG=0

###################################################
# Directories
###################################################
SRCDIR=src
EXTRA_INCDIR=../common
EXTRA_LIBDIR=
EXTRA_LIBS=
BUILDDIR=output_linux

COMMON_CPP=utils.cpp serial_linux.cpp debug.cpp
COMMON_OBJ=$(addprefix $(BUILDDIR)/common_, $(notdir $(COMMON_CPP:.cpp=.o)))

###################################################
# Supported platform list
###################################################
PLATFORM_LINUX=1
PLATFORM_MSW=0
DEF_PLATFORM=-DPLATFORM_LINUX=$(PLATFORM_LINUX) -DPLATFORM_MSW=$(PLATFORM_MSW)

###################################################
# Setup of building
###################################################
TARGET_SYSTEM=$(PLATFORM_LINUX)
CROSS_PREFIX=
CROSS_COMPILE=0

OUT_FILE=$(BUILDDIR)/$(PRJ_NAME)_linux

###################################################
# Source file list
###################################################
C_SRC=$(notdir $(wildcard $(SRCDIR)/*.c))
CPP_SRC=$(notdir $(wildcard $(SRCDIR)/*.cpp))
A_SRC=$(notdir $(wildcard $(SRCDIR)/*.s))

###################################################
# Gen object file name automatically
###################################################
C_OBJ=$(addprefix $(BUILDDIR)/, $(C_SRC:.c=.o))
CPP_OBJ=$(addprefix $(BUILDDIR)/, $(CPP_SRC:.cpp=.o))
A_OBJ=$(addprefix $(BUILDDIR)/, $(A_SRC:.s=.o))

###################################################
# Tools setup
###################################################
INCLUDES=-I$(SRCDIR) $(addprefix -I, $(EXTRA_INCDIR))

DEFINES=-DDEBUG=$(DEBUG) -DPRJ_NAME=$(PRJ_NAME) -DTARGET_SYSTEM=$(TARGET_SYSTEM) $(DEF_PLATFORM)

CC=$(CROSS_PREFIX)g++
CFLAGS=-std=c++11 -c -O -MMD -Wall $(DEFINES) $(INCLUDES)

CPP=$(CROSS_PREFIX)g++
CPPFLAGS=-std=c++11 -c -O -MMD -Wall -Wno-unused-result $(DEFINES) $(INCLUDES)

LINK=$(CROSS_PREFIX)g++
LFLAGS=$(addprefix -l, $(EXTRA_LIBS))

AR=$(CROSS_PREFIX)ar
ARFLAGS=-rc

###################################################
###################################################



###################################################
# Link file
###################################################
OBJS=$(A_OBJ) $(C_OBJ) $(CPP_OBJ) $(COMMON_OBJ)

$(OUT_FILE) : $(BUILDDIR) $(OBJS)
	@echo .
	@echo [$(OUT_FILE)]
	@$(LINK) $(OBJS) $(LFLAGS) -o $(OUT_FILE)

###################################################
# Output directory
###################################################
$(BUILDDIR) :
	@if [ ! -d $(BUILDDIR) ]; then mkdir $(BUILDDIR); fi

###################################################
# Header dependence
###################################################
$(SRCDIR)/%.rc : $(SRCDIR)/rc.h
	@touch $(SRCDIR)/$(@F)
	
-include $(BUILDDIR)/*.d

###################################################
# Compile C++ files
###################################################
$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@echo [$<]
	@$(CPP) $(CPPFLAGS) $< -o $@ -Wa,-a=$(BUILDDIR)/$(@F:.o=.lst)

$(BUILDDIR)/common_%.o : ../common/%.cpp
	@echo [$<]
	@$(CPP) $(CPPFLAGS) $< -o $@ -Wa,-a=$(BUILDDIR)/$(@F:.o=.lst)


###################################################
# Compile C files
###################################################
$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@echo [$<]
	@$(CC) $(CFLAGS) $< -o $@ -Wa,-a=$(BUILDDIR)/$(@F:.o=.lst)

###################################################
# Clear all output files
###################################################
.PHONY : clean
clean :
	@echo Delete all output files.
	@if [ -d $(BUILDDIR) ]; then rm -Rf $(BUILDDIR)/*; fi

###################################################
# Test the execute
###################################################
.PHONY : test
test : $(OUT_FILE)
	@$(OUT_FILE) --help
