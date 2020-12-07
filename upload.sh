#!/usr/bin/expect --

set timeout -1

set target_ip [lindex $argv 0]

spawn rsync -azP -zz --delete . root@$target_ip:/home/ubuntu/FakerTSN --exclude *.o --exclude *.a --exclude xmltest --exclude bin

expect "password" 

send "root\r"

interact
