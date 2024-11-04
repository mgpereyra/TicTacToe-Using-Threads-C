#!/bin/bash

# Usage: ./monitor_processes.sh [interval]
# Default interval is 2 seconds if not specified.

INTERVAL=${1:-2} # Default interval in seconds

echo "Monitoring processes every $INTERVAL seconds. Press [Ctrl+C] to stop."

while true
do
	clear
	echo "Time: $(date)"
	echo "-------------------------------------"
	# Display the list of processes
	# ps aux --sort=-%cpu #| head -n 10 # Show top 10 CPU consuming processes
	ps -eLf | grep "TicTacToe" | grep -v "grep"
	echo "-------------------------------------"
	sleep $INTERVAL
done
