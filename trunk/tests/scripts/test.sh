files=(lcc asm simulator)
if [ "$1" == "-C" ]
then
	folders=(source assembler object exec)
	existFile(){
		[[ -f "$1" ]] && echo $1 found || echo No $1 found
	} 
	existFolder(){
		if [ -d "$1" ] 
		then echo $1 folder found 
		else {
			echo No $1 found, creating $1
		 	mkdir $1 
		}
		fi
	} 
	for temp in ${files[@]}
	do
		existFile $temp
	done
	for temp in ${folders[@]}
	do
		existFolder $temp
	done
fi

cd ./source
Files="./*.c"

for f in $Files 
do
	cd ../
	cd ./assembler
	../${files[0]} -S ../source/$f
	echo compile...
	cd ../
	nameAsm=$(echo $f|sed 's/\(.*\)\.c/\1.s/')
	cd ./object	
	../${files[1]} ../assembler/$nameAsm
	echo make asm
	cd ../
	cd ./exec
	nameObj=$(echo $f|sed 's/\(.*\)\.c/\1.o/')
	../${files[2]} ../object/$nameObj
	echo execute
	cd ../ 
done
 

