.section ".text"
main:
set %r9,1	#con
set %r8,2	#con
add %r1,%r9,%r8	#ADDI2
.L1:
return
