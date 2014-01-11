#!/bin/bash
kill -15 `cat pid`
rm -rf pid nohup.out
