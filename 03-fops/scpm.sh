#! /bin/bash

file=`ls |grep .ko`
echo $file
scp $file wei@10.0.0.3:/home/wei/Projects/driver-test
