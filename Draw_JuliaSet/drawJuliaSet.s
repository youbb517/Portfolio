.text
  .global drawJuliaSet

  drawJuliaSet :
    stmfd sp!, {r4-r11, lr} @ push r4-r11, lr onto stack

	mov r6, r0      	 	@ cY
	mov r7, r1     		 	@ frame
	mov r4, #0     		 	@ r4 = 0 @ x
    moveq r5, #0   		 	@ if(eq) r5 = 0 @ y  @@@@ Conditional Execution

	adds r14, r0, r15

loop1:
    cmp r4, #640      	 	@@ x < width
	bge return_drawJuliaSet
	mov r5, #0      	 	@ r5 = 0 @@ y = 0

loop2:
    cmp r5, #480      	 	@@ y < height
	addge r4, r4, #1     	@ if( r5 >= 480 )@ x++  @@@@ Conditional Execution
	bge loop1

 @--------zx = 1500 * (x - (width>>1)) / (width>>1)---------

    ldr r0, .constant    	@ r0 = 1500
	mov r2, #640         	@ r2 = 640  @ width = 640
    mov r1, r2, asr #1   	@ r1 = width >> 1    @@@@@  Operand2
	sub r2, r4, r1       	@ r2 = x - (width>>1)
	mul r0, r0, r2    		@ r0 = 1500 * (x - (width>>1))
	bl __aeabi_idiv   		@ r0 = 1500 * (x - (width>>1)) / (width>>1)
	mov r8, r0          	@ zx @ r8 = r0 = 1500 * (x - (width>>1)) / (width>>1)

 @--------zy = 1000 * (y - (height>>1)) / (height>>1)-------

	mov r0, #1000      		@ r0 = 1000
	mov r2, #480   		 	@ r2 = 480  @ height = 480
	mov r1, r2, asr #1   	@ r1 = height >> 1
	sub r2, r5, r1   		@ r2 = y - (height>>1)
	mul r0, r0, r2   		@ r0 = 1000 * (y - (height>>1))
	bl __aeabi_idiv     	@ r0 = 1000 * (y - (height>>1)) / (height>>1)
	mov r9, r0       		@ zy  @ r9 = r0 = 1000 * (y - (height>>1)) / (height>>1)

 @---------------------- i = 255 ---------------------------

	mov r10, #255        @ r10 = 255 @ r10 is i

 @---------zx * zx + zy * zy < 4000000 && i > 0-------------

	mul r0, r8, r8      	@ r0 = zx * zx
	mul r1, r9, r9      	@ r1 = zy * zy
	add r0, r0, r1      	@ r0 = zx * zx + zy * zy
	ldr r1, .constant+4  	@ r1 = 4000000
    cmp r0, r1       		
	bge temp                @ ( r0 >= r1 ) @@ zx * zx + zy * zy > 4000000
	cmplt r10, #0      		@ ( r0 < r1 ) @@@@ Conditional Execution
	ble temp                @ ( r10 <= 0 ) 

loop3:
    mul r0, r8, r8    		@ r0 = zx * zx
	mul r1, r9, r9      	@ r1 = zy * zy
	sub r0, r0, r1      	@ r0 = zx * zx - zy * zy                 @@@@   Operand2
	mov r1, #1000      		@ r1 = 1000
	bl __aeabi_idiv     	@ r0 = (zx * zx - zy * zy)/1000
	sub r11, r0, #700    	@ r11 = (zx * zx - zy * zy)/1000 + cX    @@@@   Operand2

	mul r0, r8, r9     		@ r0 = zx * zy  
	mov r1, #2           	@ r1 = 2
	mul r0, r0, r1     		@ r0 = (2 * zx * zy)
	mov r1, #1000      		@ r1 = 1000
	bl __aeabi_idiv     	@ r0 = (2 * zx * zy)/1000
	add r0, r0, r6      	@ r0 = (2 * zx * zy)/1000 + cY
	mov r9, r0           	@ r9 = r0  @ zy = (2 * zx * zy)/1000 + cY

	mov r8, r11       		@ zx = tmp

	sub r10, r10, #1     	@ i--

	mul r0, r8, r8      	@ r0 = zx * zx
	mul r1, r9, r9     		@ r1 = zy * zy
	add r0, r0, r1      	@ r0 = zx * zx + zy * zy
	ldr r1, .constant+4  	@ r1 = 4000000
    cmp r0, r1        		@ zx * zx + zy * zy < 4000000    		
	bge temp                @ ( r0 >= r1 ) @@ zx * zx + zy * zy > 4000000
	cmplt r10, #0      		@ ( r0 < r1 ) @@@@ Conditional Execution
	ble temp                @ ( r10 <= 0 ) 
	b loop3

temp:
    and r0, r10, #0xff   	@ r0 = ( i & 0xff )
	orr r1, r0, r0, lsl #8  @ r1 = ( i & 0xff ) | ( ( i & 0xff ) << 8 )  @@@@   Operand2
	ldr r0, .constant+8  	@ r0 = 0xffff
	bic r3, r0, r1    		@ r3 = 0xffff & (~color)

	mov r0, r7              @ frame
	mov r1, #1280           @ r1 = 1280 = 2 * 640
	mul r1, r1, r5          @ r1 = 1280 * y
	add r0, r0, r1    		@ r0 = frame + 1280y
	add r0, r0, r4, lsl #1  @ r0 = frame + 1280y + 2x
	strh r3, [r0]           @ 2 byte @@ frame[y][x] = color

	add r5, r5, #1    		@ y++
	b loop2

return_drawJuliaSet:
    mov r0, #0           	@ move return code into r0
    ldmfd sp!, {r4-r11, lr} @ pop r4-r11, lr from stack
	mov pc, lr

.constant :
	.word 1500     			@ .constant+0
	.word 4000000    		@ .constant+4
	.word 0xffff    		@ .constant+8

