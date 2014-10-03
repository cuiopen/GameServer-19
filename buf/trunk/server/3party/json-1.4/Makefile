CC=gcc
SRCS=json.c json_helper.c
OBJS=$(SRCS:.c=.o)
TARGET=libjson.a

all : $(OBJS)
	$(AR) rcu $(TARGET) $^
	cp -f $(TARGET) ../libs

distclean: clean
clean:
	rm -f *.o *.a

%.o : %.c
	$(CC) -g -O2 -o $@ -c $^

