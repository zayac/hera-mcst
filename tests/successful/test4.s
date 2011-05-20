.section ".text"
main:
set %r6,-128	#con
set %r5,32766	#con
add %r8, %r0, 0	#LOADI2
add %r7, %r0, 0	#LOADI2
.L2:
clrc
add %r8,%r8,%r6	#ADDI2
.L3:
set %r12,1	#con
clrc
add %r7,%r7,%r12	#ADDI2
set %r12,256	#con
cmp %r7,%r12	#LTI2
blr .L2	#LTI2
setc
sub %r1,%r8,%r5	#SUBI2
.L1:
return
