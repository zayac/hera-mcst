#.section ".text"
main:
set %r11, 1	#con
set %r10, 2	#con
cmp %r11, %r10	#LEI2
bler .L2	#LEI2
mult %r1, %r11, %r10	#MULI2
brr .L1	#JUMPV
.L2:
add %r1, %r11, %r10	#ADDI2
.L1:
return
