#!/bin/sh

if [ "$#" -lt 1 ]
then
	echo 'provide the tar name '
	exit 0;
fi

tar cvf $1.tar package  bin 
