.section ".text"
main:
set %r8,-1	#con
set %r7,32766	#con
set %r12,7	#con
add %r8,%r8,0	#LSHI2 from emit2
lsl %r8,%r12	#LSHI2 from emit2
add %r8,%r8,0	#LSHI2 from emit2
add %r8,%r8,0	#LSHI2 from emit2
set %r12,8	#con
add %r8,%r8,0	#LSHI2 from emit2
lsl %r8,%r12	#LSHI2 from emit2
add %r8,%r8,0	#LSHI2 from emit2
add %r8,%r8,0	#LSHI2 from emit2
clrc
add %r1,%r7,%r8	#ADDI2
.L1:
return
