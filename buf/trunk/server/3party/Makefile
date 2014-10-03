
DIRS := json-1.4

all :
	@for i in $(DIRS); \
		do $(MAKE) $(OPTS) -C $$i;\
	done

clean :
	@for i in $(DIRS); \
		do $(MAKE) clean $(OPTS) -C $$i;\
	done

distclean :
	@for i in $(DIRS); \
		do $(MAKE) distclean $(OPTS) -C $$i;\
	done
