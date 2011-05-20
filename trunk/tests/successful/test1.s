.section ".text"
main:
set %r8,32767	#con
set %r7,32767	#con
setc
sub %r1,%r8,%r7	#SUBI2
.L1:
return
