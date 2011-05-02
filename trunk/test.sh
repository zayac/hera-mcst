#!/bin/bash -e

if [ ! -n "$1" ]
then
  echo "Usage: `basename $0` -B[Miss Build] -C[commit] -T [specify folder with tests]"
fi
for arg in "$@"
do 
	case $arg in
	-C)
		svn update
		svn commit -m "New message from commit"
		break
		;;
# 	-B)
# 		cd ./lcc
# 		./rbld
# 		echo LCC build success
# 		cd ../
# 
# 		make -C ./assembler clean 
# 		make -C ./assembler 
# 		echo Assembler build success
# 
# 		make -C ./simulator clean
# 		make -C ./simulator
# 		echo Simulator build success
# 		cp ./lcc/lcc ./compiler
# 		cp -R ./lcc/build ./build
# 		cp ./assembler/assembler ./asm
# 		cp ./simulator/simulator ./sim
# 		break
# 		;;
	esac
done
# 	if [ arg = "-T" ]
# 	then 
# 		echo 
# 	fi

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
	#./sim $nameObj
	
done
 

