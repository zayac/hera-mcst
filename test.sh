#!/bin/bash -e

if [ ! -n "$1" ]
then
  echo "Usage: `basename $0` -B[Miss Build] -C[commit] -T [specify folder with tests]"
fi
index=0
args=("$@")
testDir="./source"
for arg in "$@"
do 
	let "index+=1"
	case $arg in
	-C)
		svn update
		svn commit -m "New message from commit"
		break
		;;
	-T)
		testDir="${args[$index]}"
		break
		;;
	-B)
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
		cp ./lcc/lcc ./compiler
		cp -R ./lcc/build ./build
		cp ./assembler/assembler ./asm
		cp ./simulator/simulator ./sim
		;;
	esac
done
# 	if [ arg = "-T" ]
# 	then 
# 		echo 
# 	fi

for f in $testDir/*.c
do
	echo --------------------------------------Compile $f-------------------------------------- 1>> file.log
	nameAsm=$(echo $f|sed 's/\(.*\)\.c/\1.s/')
	nameObj=$(echo $f|sed 's/\(.*\)\.c/\1.o/')
	#echo -------------------$nameAsm
	#echo -------------------$nameObj
	./compiler -S $f -o $nameAsm
	cd $testDir
	echo --------------------------------------Assemble $f--------------------------------------  1>> ../file.log
	(false; (../asm ../$nameAsm 1>> ../file.log)) && true 
	if [ $? != 0 ]
	then 
		echo Error $f with  exit status 1>> ../file.log
	else
		echo Passed $f with  exit status 1>> ../file.log
	fi
	cd ../
	echo --------------------------------------Simulate $f--------------------------------------  1>> file.log
	#./sim $nameObj
	
done
 

