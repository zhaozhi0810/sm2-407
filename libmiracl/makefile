############################################################################
#makefile
############################################################################

#****************************************************************************
# Cross complie path
#****************************************************************************
#GCC_PATH=D:\msysgit\mingw
#CHAIN_ROOT=/home/yang/imax283/ctools/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin

#CROSS_COMPILE=$(CHAIN_ROOT)/arm-none-linux-gnueabi-

#CHAIN_ROOT= /home/yang/b503/ctools/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin
#CROSS_COMPILE=$(CHAIN_ROOT)/arm-linux-gnueabihf-
CROSS_COMPILE = 

CC     := $(CROSS_COMPILE)gcc
CXX    := $(CROSS_COMPILE)g++
AS	   := $(CROSS_COMPILE)as
AR     := $(CROSS_COMPILE)ar 
LD     := $(CROSS_COMPILE)ld
RANLIB := $(CROSS_COMPILE)ranlib
OBJDUMP:= $(CROSS_COMPILE)objdump
OBJCOPY:= $(CROSS_COMPILE)objcopy
STRIP  := $(CROSS_COMPILE)strip

#****************************************************************************
# Flags
#****************************************************************************

CFLAGS= -I../include
LDSCRIPT= 
LDFLAGS= 


#****************************************************************************
# Source files
#****************************************************************************
SRC_C=$(shell find . -name "*.c")

OBJ_C=$(patsubst %.c, %.o, $(SRC_C))

SRCS := $(SRC_C) $(SRC_C)

OBJS := $(OBJ_C) 


#****************************************************************************
# Targets of the build
#****************************************************************************
TARGET   	:= libmiracl

.PHONY: clean
all:  prebuild  $(TARGET).so

#****************************************************************************
# TARGET
#****************************************************************************
prebuild:
	@echo Building shared lib...

$(TARGET).so : $(OBJS)
	@echo Generating shared lib...
	$(CC) -shared -fPIC -o  $(TARGET).so $(OBJS) 
	cp $(TARGET).so ../
	@echo OK!

%.o : %.c
	$(CC) -c -fPIC $(CFLAGS) $< -o  $@
	
clean:
	@echo The following files:
	rm  -f  $(TARGET) *.so
	find . -name "*.[od]" |xargs rm
	@echo Removed!
