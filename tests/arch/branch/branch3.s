.section ".data"
.type a,#object
.size a,2
.align 2
a:
.word 0x1
.section ".text"
main:
set %r13,a	#ADDRGP2
load %r8,0,%r13	#INDIRI2
cmp %r8,%r0	#EQI2
bz .L6	#EQI2
set %r13,1	#con
cmp %r8,%r13	#EQI2
bz .L7	#EQI2
set %r13,2	#con
cmp %r8,%r13	#EQI2
bz .L6	#EQI2
set %r13,.L3	#ADDRGP2
br %r13
.L6:
set %r9,1	#con
set %r13,.L4	#ADDRGP2
br %r13
.L7:
add %r9, %r0, 0	#LOADI2
set %r13,.L4	#ADDRGP2
br %r13
.L3:
set %r9,-1	#con
.L4:
add %r1, %r9, 0	#LOADI2
.L2:
return
