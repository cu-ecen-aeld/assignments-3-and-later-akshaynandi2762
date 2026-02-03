#!/bin/bash
# Description:  start stop daemon for aesdsocket app
# Author: Akshay Nandi


DAEMON_DIR="/home/akshaynandi/course1/server"
DAEMON="$DAEMON_DIR/aesdsocket" 

# name of the daemon
DAEMON_NAME="aesdsocket"

case "$1" in
    start)
        echo "Start server in daemon mode..."
        if grep -x $DAEMON_NAME > /dev/null; then
        	echo "Error: aesdsocket is already launched !"
        	exit 1
        fi
        # Launch daemon in background with -d
        start-stop-daemon --start --background --exec $DAEMON -- -d
        ;;
    stop)    
        echo "Stop server..."
        if ! grep -x $DAEMON_NAME > /dev/null; then
		echo "Error: aesdsocket is not launched !"
		exit 1
        fi
        # Stop daemon with SIGTERM
        start-stop-daemon --stop  --signal SIGTERM --quiet --name $DAEMON_NAME
        ;;
    restart)
        $0 stop
        sleep 1
        $0 start
        ;;
    status)
        if grep -x $DAEMON_NAME > /dev/null; then
            echo "aesdsocket est actif (PID : $(grep -x $DAEMON_NAME))"
        else
            echo "aesdsocket est inactif"
        fi
        ;;
    *)
        echo "Usage: $0 {start|stop|restart|status}"
        exit 1
        ;;
        
esac

exit 0
