.section ".text"
main:
set %r8,1	#con
set %r9,2	#con
cmp %r8,%r9	#GEI2
bger .L2	#GEI2
add %r7, %r8, 0	#LOADI2
.L2:
add %r7, %r9, 0	#LOADI2
add %r1, %r7, 0	#LOADI2
.L1:
return
