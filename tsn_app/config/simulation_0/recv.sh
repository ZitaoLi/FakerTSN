#!/bin/bash

PROCESS=tsn_app
LOG="./config/simulation_0/recv.log"
DATA="./config/simulation_0/recv.data"

if [ -f "$LOG" ];then
    rm "$LOG"
fi

if [ -f "$DATA" ];then
    rm "$DATA"
fi

PID=$(ps aux| grep $PROCESS | grep -v 'grep' | awk '{print $2;}')

while [ "$PID" != "" ]
do
    if [ -f "$DATA" ];then
        echo $(cat "$DATA") >> "$LOG"
        sleep 0.001        PID=$(ps aux| grep $PROCESS | grep -v 'grep' | awk '{print $2;}')
    fi
done