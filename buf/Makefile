#####################################################################
# @file Makefile
# @brief 
#
# @version 0.4
# @author yangyoufa
#####################################################################

OS := $(shell uname)

GAME := xgame/master xgame/login xgame/record xgame/game xgame/gateway xgame/session
DIRS := 3party core common script $(GAME)

# LUAJIT -- for lua version with JIT
OPTS:=LUAJIT=true

.PHONY: all install clean distclean core release debug

all:
	@for i in $(DIRS); \
		do $(MAKE) $(OPTS) -C $$i;\
	done

install:release

clean: 
	@for i in $(DIRS); \
		do $(MAKE) clean $(OPTS) -C $$i;\
	done
	@rm -rf debug release

test:

release debug: all
	@for i in $(GAME); \
		do $(MAKE) install -C $$i; \
	done

core:
	cd core && $(MAKE)

common:
	cd common && $(MAKE)

doxy:
	$(SED) -i -e s\,INPUT\ *=.*$$,INPUT\ =\ $(DIRS),g Doxyfile 

doc:
	doxygen

