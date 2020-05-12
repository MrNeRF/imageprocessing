#!/bin/bash

if [ ! -d "build" ]
then
	echo "Creating build directory"
	mkdir build
fi

cd build
cmake ..

numberCPUs=$(lscpu | egrep "CPU\(s\):[[:space:]]*[1-9]+" | tr -dc '0-9')
numberThread=$((numberCPUs * 2))
echo "You have $numberCPUs CPUs and we use $numberThread"
make -j$numberThread
cd ..

if [ ! -h compile_commands.json ]; then
	echo "Creating symbolic link to compile_commands.json"
	ln -s build/compile_commands.json compile_commands.json
fi

ctags -R src/
