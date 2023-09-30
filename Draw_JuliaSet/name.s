.data
  msg1:  .asciz  "*****Print Name*****\n"
  team:  .asciz "Team 02\n"
  name1:  .asciz "Zhang, Yu-Qi\n"
  name2:  .asciz "Chen,  Yu-Zhen\n"
  name3:  .asciz "Zhang, Yu-Qi\n"
  msg2:  .asciz  "*****End Print*****\n"

.text
  .globl  name

  name:
    stmfd  sp!,{lr}    @ push lr onto stack
    movvc  r0, r0,ASR #0 @ Arithmetic Shift Right
    ldr    r0, =msg1
    bl     printf      @ *****Print Name*****
    mov    r1, #0      @ r1 = 0
    addhss r2, r13, #0 @ r2 = r13 + 0 and set CPSR flags
    adcs   r13,r1,r2   @ r13 = r1+r2+c and set CPSR flags
    ldr    r0, =team
    bl     printf      @ Team 02
    ldr    r0, =name1
    bl     printf      @ Zhang, Yu-Qi
    ldr    r0, =name2
    bl     printf      @ Chen, Yu-Zhen
    ldr    r0, =name3
    bl     printf      @ Zhang, Yu-Qi
    ldr    r0, =msg2
    bl     printf      @ *****End Print*****
    mov    r0, #0      @ move return code into r0
    ldmfd  sp!, {lr}   @ pop lr from stack
    mov    pc, lr

