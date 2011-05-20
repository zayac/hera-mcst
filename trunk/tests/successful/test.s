.section ".text"
main:
set %r7,30201	#con
add %r4, %r0, 0	#LOADI2
add %r3, %r0, 0	#LOADI2
add %r5, %r0, 0	#LOADI2
set %r6,2	#con
set %r8,1	#con
set %r6,2	#con
set %r12,.L5	#ADDRGP2
br %r12
.L2:
set %r8,1	#con
set %r12,.L9	#ADDRGP2
br %r12
.L6:
mult %r12,%r6,%r8	#MULI2
cmp %r12,%r7	#NEI2
bnzr .L10	#NEI2
add %r4, %r6, 0	#LOADI2
add %r3, %r8, 0	#LOADI2
set %r5,1	#con
set %r12,.L8	#ADDRGP2
br %r12
.L10:
.L7:
set %r12,1	#con
clrc
add %r8,%r8,%r12	#ADDI2
.L9:
cmp %r8,%r7	#LEI2
bler .L6	#LEI2
.L8:
set %r12,1	#con
cmp %r5,%r12	#NEI2
bnzr .L12	#NEI2
set %r12,.L4	#ADDRGP2
br %r12
.L12:
.L3:
set %r12,1	#con
clrc
add %r6,%r6,%r12	#ADDI2
.L5:
cmp %r6,%r7	#LEI2
bler .L2	#LEI2
.L4:
set %r1,1	#con
.L1:
return
