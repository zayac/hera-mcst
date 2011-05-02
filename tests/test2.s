.section ".text"
main:
set %r9,1	#con
set %r8,2	#con
cmp %r9,%r8	#LEI2
bler .L2	#LEI2
mult %r1,%r9,%r8	#MULI2
set %r13,.L1	#ADDRGP2
br %r13
.L2:
add %r1,%r9,%r8	#ADDI2
.L1:
return
