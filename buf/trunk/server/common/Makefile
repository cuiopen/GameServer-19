
OS := Linux
THDPARTY := ../3party

-include ../3party/3party.mk
-include ../buf.mk
-include ../tools.mk
-include ./common.mk

INCLS := $(INCLS)
SRCS := $(SRCS)

OBJS_ := $(SRCS:.cpp=.o)
OBJS := $(OBJS_:.c=.o)

LIBS := $(LIBS)
LIBS += -lpthread

LIBNAME = libcommon

ifeq ($(TARGETSO), true)
	TARGETS = $(LIBNAME).so
else
	TARGETS = $(LIBNAME).a
endif

.PHONY: all install clean distclean

all: $(TARGETS)

install: $(TARGETS) $(OBJS)

clean: 
	$(AT)$(RM) -f *.o $(OBJS) $(TARGETS)

$(LIBNAME).so: $(OBJS)
	$(GCC) --dynmaic $@ $^

$(LIBNAME).a: $(OBJS)
	$(AR) ur $@ $^

release: $(OBJS) $(TARGETS)

%.o: %.c
	$(AT)$(GCC) $(CFLAGS) $(INCLS) -o $@ -c $<

%.o: %.cpp
	$(AT)$(CXX) $(CPPFLAGS) $(INCLS) -o $@ -c $<

include ../deps.mk

