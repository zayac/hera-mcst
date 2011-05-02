.section ".text"
main:
set %r8,1	#con
set %r7,5	#con
add %r9, %r0, 0	#LOADI2
set %r13,.L5	#ADDRGP2
br %r13
.L2:
set %r13,7	#con
mult %r13,%r13,%r9	#MULI2
add %r8,%r8,%r13	#ADDI2
.L3:
set %r13,1	#con
add %r9,%r9,%r13	#ADDI2
.L5:
cmp %r9,%r7	#LTI2
blr .L2	#LTI2
add %r1, %r8, 0	#LOADI2
.L1:
return
