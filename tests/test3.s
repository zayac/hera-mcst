#.section ".text"
main:
set %r1, 1	#con
set %r9, 5	#con
add %r11, %r0, %r0	#LOADI2
brr .L5	#JUMPV
.L2:
set %r12, 7	#con
mult %r12, %r12, %r11	#MULI2
add %r1, %r1, %r12	#ADDI2
.L3:
set %r13, 1	#con
add %r11, %r11, %r13	#ADDI2
.L5:
cmp %r11, %r9	#LTI2
blr .L2	#LTI2
#add %r1, %r10, %r0	#LOADI2
.L1:
return
