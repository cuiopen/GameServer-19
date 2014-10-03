#####################################################################
# @file Makefile
# @brief My Makefile for resolving bugs in makefiles
#
# @version 0.4
# @author yangyoufa
#####################################################################

OS = $(shell uname)

LIBS += -lpthread

FLAGS += -W -Wall -Wwrite-strings -Wcast-qual -Wmissing-format-attribute -pedantic -Wno-long-long
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition

# avoid warning of DEBUG(...)
FLAGS += -Wno-variadic-macros
CFLAGS += -Wno-variadic-macros

FLAGS += -DNEED_MALLOC_LOG=1

#FLAGS += -DUSE_RANDOM=1
FLAGS += -DUSE_URANDOM=1

ifeq ($(OS), Linux)
FLAGS += -DUSE_EPOLL=1
FLAGS += -DUSE_EPOLL_TIMER=1
endif

ifeq ($(OS), Darwin)
FLAGS += -DUSE_KQUEUE=1
FLAGS += -DUSE_KQUEUE_TIMER=1
endif

# for edge-triggered or level-triggered in epoll
# or EV_ADD or EV_CLEAR in kqueue
# if USE_EVENT_ET=1 then edge-triggered or EV_CLEAR
# else level-triggered or EV_ADD
# default is -DUSE_EVENT_ET=0
FLAGS += -DUSE_EVENT_ET=0

# for MemPool
FLAGS += -DUSE_MEM_POOL=1
# for ObjectPool
FLAGS += -DUSE_OBJECT_POOL=1

# The size of recycle queue for MsgQueue, default is 4096
FLAGS += -DMSG_QUEUE_SIZE=4096

#
FLAGS += -DCHAT_MAX_LEN=1024

# login server
FLAGS += -DUSE_SERVER_LOGIN
FLAGS += -DUSE_SERVER_GATEWAY
FLAGS += -DUSE_SERVER_SESSION

# for sql merge, if there is an error occured
#FLAGS += -DDB_SQL_MERGE=100

# for DEFAULT_SIZE of Stream
FLAGS += -DDEFAULT_STREAM_SIZE=4096
# for DEFAULT TRUNK SIZE of Buffer
#FLAGS += -DDEFAULT_TRUNK_SIZE=64*1024
FLAGS += -DDEFAULT_BUFFER_SIZE=4096
FLAGS += -DDEFAULT_SQL_LEN=4096

FLAGS += -DUSE_QUERY_GROUP # Use thread group to query else only one thread
FLAGS += -DUSE_ZIP_DEFAULT_BUFFER=16*1024

FLAGS += -DUSING_GRIDINDEX_CELL
#FLAGS += -DUSING_GRIDINDEX_NINE
ifeq ($(OS), Darwin)
CXX0X = 
else
CXX0X = -std=c++0x
endif

ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O3 -D_NDEBUG $(FLAGS)
	CPPFLAGS += -O3 -D_NDEBUG $(FLAGS) $(CXX0X)
else
	#CFLAGS += $(FLAGS) -gdwarf-2 -g3
	#CPPFLAGS += -pg $(FLAGS) -gdwarf-2 -g3 $(CXX0X)
	CFLAGS += $(FLAGS) -gdwarf-2 -g
	CPPFLAGS += -pg $(FLAGS) -gdwarf-2 -g $(CXX0X)
endif

ifeq ($(OS), Darwin)
	CFLAGS += -pagezero_size 0x10000
	CPPFLAGS += -pagezero_size 0x10000
endif

INCLS += `mysql_config --include`
INCLS += `xml2-config --cflags`

