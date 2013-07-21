
# Define the applications properties here:

TARGET = PocketSNES

CC  := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
STRIP := $(CROSS_COMPILE)strip
SDL_CONFIG ?= sdl-config

INCLUDE = -I pocketsnes \
		-I sal/linux -I sal/linux/include \
		-I sal/common -I sal/common/includes \
		-I pocketsnes/include \
		-I menu -I pocketsnes/linux -I pocketsnes/snes9x

CFLAGS = $(INCLUDE) -DRC_OPTIMIZED -D__LINUX__ -D__DINGUX__ \
		 -g -O3 -pipe -ffunction-sections -ffast-math \
		 $(shell $(SDL_CONFIG) --cflags) \
		 #-flto -fwhole-program #-fsigned-char

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti

LDFLAGS = $(CXXFLAGS) -lpthread -lz -lpng -lm -lgcc \
		  $(shell $(SDL_CONFIG) --libs)

# Find all source files
SOURCE = pocketsnes/snes9x menu sal/linux sal/common
SRC_CPP = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.cpp))
SRC_C   = $(foreach dir, $(SOURCE), $(wildcard $(dir)/*.c))
OBJ_CPP = $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_C   = $(patsubst %.c, %.o, $(SRC_C))
OBJS    = $(OBJ_CPP) $(OBJ_C)

.PHONY : all
all : $(TARGET)

.PHONY: opk
opk: $(TARGET).opk

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(TARGET).opk: $(TARGET)
	rm -rf .opk_data
	cp -r data .opk_data
	cp $< .opk_data/pocketsnes.gcw0
	$(STRIP) .opk_data/pocketsnes.gcw0
	mksquashfs .opk_data $@ -all-root -noappend -no-exports -no-xattrs

.PHONY : clean
clean :
	rm -f $(OBJS) $(TARGET)
	rm -rf .opk_data $(TARGET).opk
