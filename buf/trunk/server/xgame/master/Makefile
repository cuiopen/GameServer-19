
OS := Linux

include ../../buf.mk
include ../../tools.mk
include ./master.mk

SRCS := $(SRCS)

OBJS_ := $(SRCS:.cpp=.o)
OBJS := $(OBJS_:.c=.o)
CFG := config.lua
DEPS := ../../common/libcommon.a ../../core/libcore.a

TARGET = master

.PHONY: all install clean distclean

all: $(TARGET) $(DEPS)

debug: $(TARGET) $(DEPS)

$(TARGET): $(OBJS) $(DEPS)
	$(AT)$(ECHO) "debug" > .pub
	$(AT)$(CXX) $(CPPFLAGS) $(INCLS) -o $@ $^ $(LIBS)

#$(AT)$(INSTALL) ../../common/$(CFG) ../`$(CAT) .pub`/config/$(CFG)
install: $(TARGET)
	$(AT)if [ ! -d ../`$(CAT) .pub` ]; then $(MKDIR) ../`$(CAT) .pub`; fi
	$(AT)if [ ! -d ../`$(CAT) .pub`/config ]; then $(MKDIR) ../`$(CAT) .pub`/config; fi
	$(AT)$(INSTALL) $(TARGET) ../`$(CAT) .pub`/$(TARGET)
	$(AT)$(INSTALL) ../../common/$(CFG) config/$(CFG)
	$(AT)if [ release = `$(CAT) .pub` ]; then $(AT)$(INSTALL) $(TARGET).symbol ../`$(CAT) .pub`/$(TARGET).symbol; fi

clean:
	$(AT)$(RM) -f *.o $(OBJS) $(TARGET) $(TARGET).symbol

release: $(TARGET)
	$(AT)$(ECHO) "release" > .pub
	$(AT)$(OBJCOPY) --only-keep-debug $(TARGET) $(TARGET).symbol
	$(AT)$(OBJCOPY) --strip-debug $(TARGET)

%.o: %.c
	$(AT)$(GCC) $(CFLAGS) $(INCLS) -o $@ -c $<

%.o: %.cpp
	$(AT)$(CXX) $(CPPFLAGS) $(INCLS) -o $@ -c $<

include ../../deps.mk

