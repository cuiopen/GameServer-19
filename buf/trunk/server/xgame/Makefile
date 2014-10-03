#####################################################################
# @file Makefile
# @brief 
#
# @version 0.4
# @author yangyoufa
#####################################################################

OS := Linux

include ../buf.mk
include ../tools.mk

dirs = login master gateway session scene record
CFG := config.lua

.PHONY: all install clean distclean release

all: 
	$(AT)$(ECHO) "debug" > .pub
	@for d in $(dirs); do make -C $$d; done

install: 
	$(AT)if [ ! -d `$(CAT) .pub` ]; then $(MKDIR) `$(CAT) .pub`; fi
	$(AT)if [ ! -d `$(CAT) .pub`/config ]; then $(MKDIR) `$(CAT) .pub`/config; fi
	$(AT)$(INSTALL) ../common/$(CFG) `$(CAT) .pub`/config/$(CFG)
	@for d in $(dirs); do make install -C $$d; done

clean: 
	@for d in $(dirs); do make clean -C $$d; done

distclean:
	@for d in $(dirs); do make distclean -C $$d; done

release:
	$(AT)$(ECHO) "release" > .pub
	@for d in $(dirs); do make release -C $$d; done

