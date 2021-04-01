#!/bin/bash

PROCESS=tsn_app
LOG="./config/simulation_0/mem.log"

#删除上次的监控文件
if [ -f "$LOG" ];then
    rm "$LOG"
fi

#过滤出需要的进程ID
PID=$(ps aux| grep $PROCESS | grep -v 'grep' | awk '{print $2;}')

while [ "$PID" != "" ]
do
    cat /proc/$PID/status | grep RSS | tr -cd "[0-9]" >> "$LOG"
    echo >> "$LOG"
    sleep 1
    PID=$(ps aux| grep $PROCESS | grep -v 'grep' | awk '{print $2;}')
done