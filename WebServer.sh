#!/bin/sh

#the script to control the app
#by heat@2016.6.26

appname=WebServer

start() {

	echo "Now will start the $appname" 
}

stop() {
	
	echo "Now will stop the $appname"
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
