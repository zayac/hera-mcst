#!/bin/bash -e

if [ "$1" != "-B" ]
then
    cd ./lcc
    ./rbld
    echo LCC build success
    cd ../

    make -C ./assembler clean 
    make -C ./assembler 
    echo Assembler build success

    make -C ./simulator clean
    make -C ./simulator
    echo Simulator build success
fi
cp ./lcc/lcc ./compiler
cp -R ./lcc/build ./build
cp ./assembler/assembler ./asm
cp ./simulator/simulator ./sim

svn update
svn commit -m "New message from commit"

for f in ./source/*.c
do
	echo --------------------------------------Compile $f--------------------------------------
	nameAsm=$(echo $f|sed 's/\(.*\)\.c/\1.s/')
	nameObj=$(echo $f|sed 's/\(.*\)\.c/\1.o/')
	#echo -------------------$nameAsm
	#echo -------------------$nameObj
	./compiler -S $f -o $nameAsm
	cd ./source
	echo --------------------------------------Assemble $f--------------------------------------
	../asm ../$nameAsm
	cd ../
	echo --------------------------------------Simulate $f--------------------------------------
	./sim $nameObj
	
done
 

