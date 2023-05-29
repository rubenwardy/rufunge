#!/bin/bash

rm -rf /tmp/rufunge
mkdir /tmp/rufunge
mkdir /tmp/rufunge/examples

for name in examples/*.rf; do
	./bin/a $name > /tmp/rufunge/$name.txt 2> ./log.txt

	echo Result for $name:
	cat /tmp/rufunge/$name.txt

	if cmp --silent $name.txt /tmp/rufunge/$name.txt; then
		echo "Test passed: $name"
	else
		/bin/echo -e "\e[1;31mTest failed: $name\e[0m"
		exit -1
	fi
done

/bin/echo -e "\e[1;32mAll Tests Passed\e[0m"
