#!/bin/sh

#the script to control the app
#by heat@2016.6.26

appname=WebServer

start() 
{

	echo "Now will start the $appname" 

	pid=$(pidof $appname)

	if [ -n "$pid" ]; then
		echo "the $appname is running,start failed,pid is $pid"
		exit 0
	fi

	echo "Now run the $appname"

	nohup ./$appname  > /dev/null  2>&1 &
	apppid=$(pidof $appname)
	echo "the $appname is start successful! the pid is $apppid"
}

stop() 
{
	
	echo "Now will stop the $appname"

	pid=$(pidof $appname)
	if [ -z "$pid" ]; then
		echo "the app is not running"
		exit 
	fi

	kill -9 $pid
	
	pid=$(pidof $appname)

	if [ -z "$pid" ];then
		echo " kill successful "
	fi
}

case "$1" in
	start)
		start
	;;
	stop)
		stop
	;;
	*)
	echo $"Usage: $0{start|stop|view|restart}"	
	exit 1
esac

exit $?
