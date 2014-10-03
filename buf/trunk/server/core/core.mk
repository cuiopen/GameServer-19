
INCLS 	+= 	-I.
INCLS 	+= -I/opt/local/include/
LIBS 	+= -L/opt/local/lib

CORE 	:= 	RTime.cpp New.cpp
CORE 	+= 	Logger.cpp \
			Properties.cpp \
			Zip.cpp \
			Random.cpp \
			ThreadGroup.cpp \
			Utils.cpp \
			Socket.cpp \
			TcpClient.cpp \
			TcpConnection.cpp \
			TcpProcessor.cpp \
			TcpProcessMgr.cpp \
			TcpProcessPool.cpp \
			TcpProcessorPool.cpp \
			TcpConnectionPool.cpp \
			TcpServer.cpp \
			Event.cpp \
			Url.cpp \
			DBDescription.cpp \
			DBMysqlConnection.cpp \
			DBConnectionPool.cpp \
			DBMysqlConnectionPool.cpp \
			DBDescription.cpp \
			INI.cpp \
			Path.cpp \
			File.cpp \
			Timer.cpp \
			MCached.cpp \
			MemPool.cpp \
			WordFilter.cpp \
			JsonParser.cpp

ifeq ($(OS), Linux)
CORE 	+= 	linux/Encoding_POSIX.cpp \
			linux/Thread_POSIX.cpp \
			linux/Socket_POSIX.cpp \
			linux/TcpConnection_POSIX.cpp \
			linux/TcpServer_POSIX.cpp \
			linux/Path_POSIX.cpp \
			linux/File_POSIX.cpp \
			linux/Event_EPOLL.cpp \
			linux/XML_POSIX.cpp \
			linux/System.cpp
endif

ifeq ($(OS), Darwin)
CORE 	+= 	linux/Encoding_POSIX.cpp \
			linux/Thread_POSIX.cpp \
			linux/Socket_POSIX.cpp \
			linux/TcpConnection_POSIX.cpp \
			linux/TcpServer_POSIX.cpp \
			linux/Path_POSIX.cpp \
			linux/File_POSIX.cpp \
			linux/XML_POSIX.cpp \
			linux/System.cpp \
			mac/Event_KQUEUE.cpp
endif

SRCS 	:= 	$(CORE)

