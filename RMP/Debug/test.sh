#!/bin/bash

./RMP ../Obstacle1.png 100 50 1050 750 -test
if [ $? -ne 0 ]; then
	echo "test failed"
    else
	echo "test 1/5 succeded"
fi

./RMP ../Obstacle2.png 100 50 1000 600 -test
err=$?
if [ $err -ne 1 ]; then
	echo "test failed return was $err instead of 1"
    else
	echo "test 2/5 succeded"
fi

./RMP ../Obstacle3.png 50 150 1050 750 -test
if [ $? -ne 0 ]; then
	echo "test failed"
    else
	echo "test 3/5 succeded"
fi

./RMP ../Obstacle4.png 50 150 1050 750 -test
if [ $? -ne 1 ]; then
	echo "test failed"
    else
	echo "test 4/5 succeded"
fi

./RMP ../Obstacle5.png 50 150 1050 750 -test
if [ $? -ne 0 ]; then
	echo "test failed"
    else
	echo "test 5/5 succeded"
fi







