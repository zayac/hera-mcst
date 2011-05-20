.section ".text"
main:
set %r8,32767	#con
set %r7,-32767	#con
clrc
add %r1,%r8,%r7	#ADDI2
.L1:
return
