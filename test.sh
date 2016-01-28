#!/bin/bash

rm -rf /tmp/rufunge
mkdir /tmp/rufunge
mkdir /tmp/rufunge/examples

for name in examples/*.rf; do
	./bin/a $name > /tmp/rufunge/$name.txt

	echo Result for $name:
	cat /tmp/rufunge/$name.txt

	if cmp --silent $name.txt /tmp/rufunge/$name.txt; then
		echo "Test passed: $name"
	else
		echo "Test failed: $name"
		exit -1
	fi
done
