#####################################################################
# @file Makefile
# @brief 
#
# @version 0.4
# @author yangyoufa
#####################################################################

OS := $(shell uname)
THDPARTY := ../3party

include ../tools.mk
include ../3party/3party.mk
include ../buf.mk

INCLS += -I.. -I../xgame/common -I../xgame/generated -I/opt/local/include/
LIBS  += -L../core -L../script -L/opt/local/lib/
LIBS  += -L../common
LIBS  += -lcommon -lcore -lscript -ldl -lz -lpthread -lmemcached -ljson
ifeq ($(OS), Darwin)
LIBS += -liconv
endif

FLAGS  += -W -Wall -Wwrite-strings -Wcast-qual -Wmissing-format-attribute -pedantic -Wno-long-long
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition

INCLS += $(shell xml2-config --cflags)
INCLS += $(shell mysql_config  --cflags)
FLAGS += $(INCLS)

LIBS += $(shell xml2-config --libs)
LIBS += $(shell mysql_config  --libs_r)

#CXX0X = -std=c++0x
CXX0X = 

ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O2 -DNDEBUG $(FLAGS)
	CPPFLAGS += -O2 -DNDEBUG $(FLAGS) $(CXX0X) 
else
	CFLAGS += -g $(FLAGS)
	CPPFLAGS += -g -pg $(FLAGS) $(CXX0X)
endif

TARGETS = $(shell find *.cpp | sed s/\.cpp//g)

.PHONY: all clean distclean release

all: $(TARGETS)
%: %.cpp ../core/libcore.a ../script/libscript.a ../common/libcommon.a
	$(AT)$(CXX) $(CPPFLAGS) $(INCLS) -o $@ $^ $(LIBS)

release:all

clean: 
	$(AT)$(RM) -f *.o $(TARGETS)

#include ../deps.mk

