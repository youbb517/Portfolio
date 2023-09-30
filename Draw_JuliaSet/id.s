.data
  msg1:  .asciz "*****Input ID*****\n"
  msg2:  .asciz "** Please Enter Member 1 ID: **\n"
  msg3:  .asciz "** Please Enter Member 2 ID: **\n"
  msg4:  .asciz "** Please Enter Member 3 ID: **\n"
  msg5:  .asciz "** Please Enter Command **\n"
  msg6:  .asciz "*****Print Team Member ID and ID Summation*****\n"
  msg7:  .asciz "\n\nID Summation = %d\n"
  msg8: .asciz "*****End Print*****\n"
  Input: .asciz "%d"
  Output:  .asciz "%d\n"
  Command: .asciz "%s"
  p:  .asciz "p\0\0\0"

  id1:   .word 0
  id2:   .word 0
  id3:   .word 0
  sum:   .word 0
  Commandspace: .word 0

.text
  .globl  id

  NotP:
    ldr    r0, =msg8   @ *****End Print*****
    bl     printf
    ldmfd  sp!,{r7-r10, lr}   @ pop r7-r10, lr from stack
    mov    pc,  lr

  id:
    stmfd  sp!,{r7-r10, lr} @ push r7-r10, lr onto stack
 
	mov    r7, r0   @ r7 = r0 
	mov    r8, r1   @ r8 = r1
	mov    r9 , r2  @ r9 = r2 
	mov    r10, r3  @ r10 = r3
 
    mov    r3 , #0
    ldr    r0, =msg1
    bl     printf   @ *****Input ID*****

    ldr    r0, =msg2
    bl     printf   @ ** Please Enter Member 1 ID: **
    ldr    r0, =Input
    ldr    r1, =id1
    bl     scanf

    ldr    r0, =msg3
    bl     printf   @ ** Please Enter Member 2 ID: **
    ldr    r0, =Input
    ldr    r1, =id2
    bl     scanf

    ldr    r0, =msg4
    bl     printf   @ ** Please Enter Member 3 ID: **
    ldr    r0, =Input
    ldr    r1, =id3
    bl     scanf

    @@@@@@@@@@@@@@@@@

    ldr    r1, =id1     @ load address of id1
    ldr    r1, [r1]     @ load value of id1 in r1
    ldr    r2, =id2     @ load address of id2
    ldr    r2, [r2]     @ load value of id2 in r2
    addpl  r1,r1,r2     @ if( N clear ) r1 = r1 + r2
    ldr    r2, =id3     @ load address of id3
    ldr    r2, [r2]     @ load value of id3 in r2
    addpl  r1,r1,r2     @ if( N clear ) r1 = r1 + r2
    ldr    r2, =sum     @ load address of sum
    str    r1, [r2]     @ store in r1 at the r2 address

	@@@@@@@@@@@@@@@@@

    ldr   r1, =id1   	@ load address of id1
	ldr   r1, [r1]      @ load value of id1 in r1
    str   r1, [r7]      @ store in r1 at the r7 address

    ldr   r1, =id2      @ load address of id2
	ldr   r1, [r1]		@ load value of id2 in r1
    str   r1, [r8]		@ store in r1 at the r8 address

    ldr   r1, =id3		@ load address of id3
	ldr   r1, [r1]		@ load value of id3 in r1
    str   r1, [r9]		@ store in r1 at the r9 address
 
    ldr   r1, =sum		@ load address of sum
	ldr   r1, [r1]		@ load value of sum in r1
    str   r1, [r10]		@ store in r1 at the r10 address
 
    @@@@@@@@@@@@@@@@@

    ldr    r0, =msg5
    bl     printf   @ ** Please Enter Command **

    ldr    r0, =Command
    ldr    r1, =Commandspace
    bl     scanf

    ldr    r0, =p   @ load address of p
    ldr    r0, [r0] @ load value of p in r0
    ldr    r1, =Commandspace  @ load address of Commandspace
    ldr    r1, [r1] @ load value of Commandspace in r1
    cmp    r0, r1  @ compare them
    blne   NotP    @ if( Commandspace != "p" ) branch to NotP

    @@@@@@@@@@@@@@@@@

    ldreq  r0, =msg6
    bl     printf  @*****Print Team Member ID and ID Summation*****

    ldr    r0, =Output
    ldr    r1, =id1
    ldr    r1, [r1]
    bl     printf

    ldr    r0, =Output
    ldr    r1, =id2
    ldr    r1, [r1, #0]
    bl     printf

    ldr    r0, =Output
    ldr    r1, =id3
    mov    r3, #0
    ldr    r1, [r1, r3]
    bl     printf

    ldr    r0, =msg7   @ ID Summation = %d
    ldr    r1, =sum
    mov    r3, #0
    ldr    r1, [r1, r3, ror #0]
    bl     printf

    ldr    r0, =msg8
    bl     printf      @ *****End Print*****

    mov    r0,#0       @ move return code into r0
    ldmfd  sp!,{r7-r10, lr}   @ pop r7-r10, lr from stack
    mov    pc, lr
   
