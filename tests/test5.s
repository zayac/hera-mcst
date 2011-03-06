#.section ".text"
main:
set %r11, 1	#con
set %r10, 11	#con
br .L3	#JUMPV
.L2:
set %r13, 1	#con
add %r11, %r11, %r13	#ADDI2
sub %r10, %r10, %r13	#SUBI2
.L3:
cmp %r11, %r10	#LTI2
bl .L2	#LTI2
add %r1, %r0, 0	#LOADI2
.L1:
return
