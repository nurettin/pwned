#!/bin/bash
phantomjs --webdriver=16666 &
sleep 1s
if [ $? -ne 0 ] ; then
  printf "process failed!\n";
else
  printf "process started!\n";
  echo $! > pid;
fi
