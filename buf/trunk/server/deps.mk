.PHONY: distclean

ifneq ($(OS),Windows)
.deps/%.d: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM -MP -MT $(patsubst %.cpp,%.o,$<) $(CPPFLAGS) $(INCLS) $< -MF $@

.deps/%.d: %.c
	@mkdir -p $(dir $@)
	$(CXX) -MM -MP -MT $(patsubst %.c,%.o,$<) $(CPPFLAGS) $(INCLS) $< -MF $@

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(addprefix .deps/,$(SRCS:.cpp=.d))
-include $(addprefix .deps/,$(SRCS:.c=.d))
endif
endif

distclean: clean
	$(RM) -rf  .deps $(TARGETS)
	$(RM) -rf *.stackdump *.core *.symbol
else
distclean:
endif

