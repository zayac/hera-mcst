#.section ".text"
main:
set %r11, 1	#con
set %r10, 2	#con
add %r1, %r11, %r10	#ADDI2
.L1:
return
