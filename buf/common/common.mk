
INCLS += 	-I.
INCLS += 	-I/usr/include/openssl
INCLS += 	-I../

ifeq ($(OS),Linux)
INCLS += 	`xml2-config --cflags`
LIBS += 	`xml2-config --libs`
else
INCLS += 	
LIBS += 	
endif

SRCS 	:= 	$(shell find *.cpp)

