#.section ".text"
main:
set %r10, 1	#con
set %r11, 2	#con
cmp %r10, %r0	#EQI2
bz .L4	#EQI2
set %r13, 1	#con
cmp %r10, %r13	#EQI2
bz .L5	#EQI2
br .L2	#JUMPV
.L4:
set %r11, 5	#con
br .L3	#JUMPV
.L5:
set %r11, 10	#con
br .L3	#JUMPV
.L2:
set %r11, 15	#con
.L3:
add %r1, %r11, 0	#LOADI2
.L1:
return
