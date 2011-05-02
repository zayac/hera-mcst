.section ".text"
main:
set %r8,1	#con
set %r9,2	#con
cmp %r8,%r0	#EQI2
bzr .L4	#EQI2
set %r13,1	#con
cmp %r8,%r13	#EQI2
bzr .L5	#EQI2
set %r13,.L2	#ADDRGP2
br %r13
.L4:
set %r9,5	#con
set %r13,.L3	#ADDRGP2
br %r13
.L5:
set %r9,10	#con
set %r13,.L3	#ADDRGP2
br %r13
.L2:
set %r9,15	#con
.L3:
add %r1, %r9, 0	#LOADI2
.L1:
return
