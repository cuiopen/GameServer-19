
INCLS += 	-I.
INCLS += 	-I/usr/include/openssl
INCLS += 	-I../../

ifeq ($(OS),Linux)
INCLS += 	`xml2-config --cflags`
LIBS += 	`xml2-config --libs`
else
INCLS += 	
LIBS += 	
endif

LIBS += -L../../common -L../../core -L../../script
LIBS += -lcore -lcommon -lcore -lcommon -lscript -ldl -lz -lpthread

SRCS 	:= 	$(shell find *.cpp)

