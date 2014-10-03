#i=1;while [ $i -le 2 ]; do i=`expr $i + 1`; ./tcpconn & done
#i=1;while [ $i -le 100 ]; do i=`expr $i + 1`; ./tcpconn & done
#i=1;while [ $i -le 50 ]; do i=`expr $i + 1`; ./tcpconn & done
#i=1;while [ $i -le 10 ]; do i=`expr $i + 1`; ./tcpconn & done
i=1;while [ $i -le 100 ]; do i=`expr $i + 1`; ./tcpclient & done
#i=1;while [ $i -le 2 ]; do i=`expr $i + 1`; ./tcpclient & done
