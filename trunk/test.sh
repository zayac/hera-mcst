#!/bin/bash -e

if [ "$1" == "-B" ]
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
cp ./lcc/lcc ./
cp ./assembler/assembler ./
cp ./simulator/simulator ./


cd ./source
Files="./*.c"

for f in $Files 
do
	echo $OPTIND 
	echo $f
	cd ../lcc
	./lcc -S ../source/$f
	#nameAsm=$(echo $f|sed 's/\(.*\)\.c/\1.s/')
	#cd ./object	
	#../${files[1]} ../assembler/$nameAsm
	#echo make asm
	#cd ../
	#cd ./exec
	#nameObj=$(echo $f|sed 's/\(.*\)\.c/\1.o/')
	#../${files[2]} ../object/$nameObj
	#echo execute
	#cd ../ 
done
 

