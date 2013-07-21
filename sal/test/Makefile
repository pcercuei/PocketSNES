
# Define the applications properties here:

APP_NAME = test

# Define the compiler settings here:

CPP       = g++
CC        = gcc
LD        = ld

SOURCE    = . ../linux ../common

INCLUDE   = -I../linux -I../linux/include \
		-I../common -I../common/includes -I$

W_OPTS    = -Wall -Wextra -finline-functions -fomit-frame-pointer -msoft-float -fno-builtin -fno-exceptions -D__UNIX__
CPP_OPTS  = -O3 $(INCLUDE) -mips32 -fno-pic $(W_OPTS) -D_DEBUG -DMPU_JZ4740 -c
CC_OPTS   = -O3 $(INCLUDE) -mips32 -fno-pic $(W_OPTS) -D_DEBUG -DMPU_JZ4740 -c
CC_OPTS_A = $(CC_OPTS) -D_ASSEMBLER_

#LIB_PATH  = $(DINGOO_SDK)/lib
LIBS      = -lz -lpng -lc -lsml -lm -lgcc

LD_SCRIPT = $(DINGOO_SDK)/lib/dingoo.xn
LD_OPTS   = -nodefaultlibs --script $(LD_SCRIPT) $(LIBS) -o $(APP_NAME).elf

# Find all source files

SRC_CPP = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.cpp))
SRC_C   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.c))
SRC_S   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.S))
OBJ_CPP = $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_C   = $(patsubst %.c, %.o, $(SRC_C))
OBJ_S   = $(patsubst %.S, %.o, $(SRC_S))
OBJ     = $(OBJ_CPP) $(OBJ_C) $(OBJ_S)

# Compile rules.

.PHONY : all

all : $(APP_NAME).app

$(APP_NAME).sim : $(APP_NAME).app
	cp $(APP_NAME).app $(APP_NAME).sim

$(APP_NAME).app : $(APP_NAME).elf
	$(DINGOO_SDK)/tools/elf2app/elf2app $(APP_NAME)

$(APP_NAME).elf : $(OBJ)
	$(LD) $(OBJ) $(LD_OPTS)

$(OBJ_CPP) : %.o : %.cpp
	$(CPP) $(CPP_OPTS) -o $@ $<

$(OBJ_C) : %.o : %.c
	$(CC) $(CC_OPTS) -o $@ $<

$(OBJ_S) : %.o : %.S
	$(CC) $(CC_OPTS_A) -o $@ $<

# Clean rules

.PHONY : clean

clean :
	rm -f $(OBJ) $(APP_NAME).elf $(APP_NAME).bin $(APP_NAME).app


