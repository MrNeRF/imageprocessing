#!/bin/bash

if [ ! -d "build" ]
then
	echo "Creating build directory"
	mkdir build
fi

cd build
cmake ..

make -j8
cd ..

if [ ! -h compile_commands.json ]; then
	echo "Creating symbolic link to compile_commands.json"
	ln -s build/compile_commands.json compile_commands.json
fi

ctags -R src/
