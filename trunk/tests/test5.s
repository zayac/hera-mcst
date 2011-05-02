.section ".text"
main:
set %r9,1	#con
set %r8,11	#con
set %r13,.L3	#ADDRGP2
br %r13
.L2:
set %r13,1	#con
add %r9,%r9,%r13	#ADDI2
sub %r8,%r8,%r13	#SUBI2
.L3:
cmp %r9,%r8	#LTI2
blr .L2	#LTI2
add %r1, %r0, 0	#LOADI2
.L1:
return
