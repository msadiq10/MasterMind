@ Used the CPUlator to run and debug your code: https://cpulator.01xz.net/?sys=arm-de1soc&d_audio=48000
@ Note, that the CPUlator simulates a DE1-SoC device, and here you should use the HEX displays to show the numbers
@ See the Tutorials on LED, Button, and HEX displays in the F28HS course (Weeks 8 and 9)
@
@ This ARM Assembler code should implement a matching function, for use in MasterMind program, as
@ described in the CW3 specification. It should produce as output 2 numbers, the first for the
@ exact matches (peg of right colour and in right position) and approximate matches (peg of right
@ color but not in right position). Make sure to count each peg just once!
@	
@ Example (first sequence is secret, second sequence is guess):
@ 1 2 1
@ 3 1 3 ==> 0 1
@ Display the result as two digits on the two rightmost HEX displays, from left to right
@
@
@ -----------------------------------------------------------------------------
@
@ Registers used for the following program:
@
@ R0 - pointer to secret sequence, then holding the exact matches towards the end of 'match' sub-routine
@ R1 - pointer to guess sequence, then holding the approxiamte matches towards the end of 'match' sub-routine
@ R4 - used main as a loop variable in 'match' sub-routine, then to load 'HEXBASE' in 'showMatches' sub-routine
@ R5 - used to store exact matches counter in 'match' sub-routine, then load the digits table in 'showMatches' sub-routine
@ R6 - used to load values of elements in secret sequence in 'match' sub-routine, then binary value for exact matches in the digits table in 'showMatches' sub-routine
@ R7 - used to load values of elements in guess sequence in 'match' sub-routine, then binary value for approximate matches in the digits table in 'showMatches' sub-routine
@ R8 - used to store approximate matches counter in 'match' sub-routine
@ R10 - used as inner loop variable of the nested loop for approximate matches in 'match' sub-routine
@
@ -----------------------------------------------------------------------------
.text
.global         main
main: NOP
	LDR  R2, =secret		@ pointer to secret sequence
	LDR  R3, =guess			@ pointer to guess sequence

	@ you probably need to initialise more values here

	MOV R0, R2				@ move pointer to secret sequence into R0
	MOV R1, R3				@ move pointer to guess sequence into R1

	@ ... COMPLETE THE CODING BY ADDING YOUR CODE HERE, you may want to use sub-routines to structure your code

	BL match				@ sub-routine to returns the number of exact and approxiamte matches in registers R0 and R1
	BL showMatches			@ sub-routine to display the exact and approxiamte matches onto the HEX-display
	B exit					@ sub-routine to exit the program

match:	NOP
	PUSH {R4-R8, R10, LR}	@ push registers r4-r8, r10 and lr to stack
	MOV R4, #0				@ loop variable 
	MOV R5, #0				@ exact matches counter

exactMatches:	NOP
	CMP R4, #LEN			@ check if we have reached end of the sequence
	BGE approxMatches		@ if yes, then move to approximate matches part
	LDR R6, [R0]			@ else, load values of R0 and R1 into R6 and R7, respectively.
	LDR R7, [R1]
	CMP R6, R7				@ check if current elements of secret and guess are equal 
	BEQ incrExact			@ if yes, increment exact matches counter 
	ADD R0, #0x04			@ move to the next element in secret
	ADD R1, #0x04			@ move to the next element in guess
	ADD R4, #0x1			@ increment loop variable by 1
	B exactMatches			@ loop to continue the same for next elements

incrExact:	NOP
	ADD R5, #0x1			@ increment the exact match counter by 1
	MOV R6, #0				@ set the current elements of secret and guess 
	MOV R7, #0				@ to zero to avoid multiple checks.
	STR R6, [R0]			@ put them back into the sequences
	STR R7, [R1]
	ADD R0, #0x04			@ move to the next element in secret
	ADD R1, #0x04			@ move to the next element in guess
	ADD R4, #0x1			@ increment loop variable by 1
	B exactMatches			@ loop to continue the same for next elements

approxMatches:	NOP
	MOV R4, #0          	@ new loop variable
	LDR R0, =secret	    	@ pointer to secret sequence
	LDR R1, =guess	    	@ pointer to guess sequence
	MOV R8, #0          	@ approximate matches counter

outerLoop:	NOP	
	MOV R10, #0         	@ inner loop variable 
	CMP R4, #LEN        	@ check if we have reached end of the guess sequence
	BGE exitloop	    	@ if yes, then exit the loop.
	LDR R6, [R1]        	@ else, load value of R1 into R6.
	CMP R6, #0				@ if the current element of the guess sequence is 0, skip it
	BEQ skip	
	B innerLoop				@ move into the inner loop

skip:	NOP	
	ADD R1, #0x04			@ move to the next element in the guess sequence
	ADD R4, #0x1        	@ increment the outer loop variable by 1
	B outerLoop     		@ branch back to the outer loop

innerLoop:	NOP
	CMP R10, #LEN       	@ check if we have reached end of the secret sequence
	BGE exitInnerLoop		@ if yes, then exit this nested loop and go back to the main loop
	LDR R7, [R0]        	@ else, load value of R2 into R6.
	CMP R6, R7          	@ check if current elements of secret and guess are equal 
	BEQ incrApprox    		@ if yes, increment approximate matches counter 
	ADD R10, #0x1       	@ increment nested loop variable by 1 
	ADD R0, #0x04       	@ move to the next element in the secret sequence
	B innerLoop         	@ loop back to the inner loop

exitInnerLoop:	NOP
	ADD R1, #0x04			@ move to the next element in the guess sequence
	ADD R4, #0x1			@ increment the outer loop variable by 1
	LDR R0, =secret			@ pointer to secret sequence
	B outerLoop				@ branch back to the outer loop

incrApprox:	NOP
	MOV R7, #0				@ set the current element of secret sequence to zero
	STR R7,[R0]				@ to avoid multiple checks
	LDR R0, =secret			@ pointer to secret sequence
	ADD R8, #0x1			@ increment approximate matches counter by 1
	ADD R1, #0x04			@ move to the next element in the guess sequence
	ADD R4, #0x1			@ increment loop variable by 1
	B outerLoop				@ branch back to outer loop

exitloop:	NOP
	MOV R0, R5				@ move exact matches into R0
	MOV R1, R8				@ move approximate matches into R1
	POP {R4-R8, R10, LR}	@ pop registers r4-r8, r10 and lr from stack
	BX LR					@ branch back to main

showMatches:	NOP
	PUSH {R4-R7, LR}		@ push registers r4-r7 and lr into the stack
	LDR R4, =HEXBASE		@ load physical address of the HEX displays into R4	
	LDR R5, =digits 		@ load the digits table into R5
	LDRB R6, [R5,R0]		@ finding the binary value corresponding to exact matches in the digits table, and loading into R6
	LSL R6, #8 				@ Left shift by 8 to get to the 2nd place from right of the hex-display
	LDR R5, =digits  		@ load the digits table into R5
	LDRB R7, [R5,R1]    	@ finding the binary value corresponding to approximate matches in the digits table, and loading into R7
	ORR R6, R7   			@ OR both the values and store in R6
	STR R6, [R4]  			@ store R6 into R4 to display on HEX display
	POP {R4-R7, LR}			@ pop registers r4-r7 and lr from the stack
	BX LR					@ branch back to the main

exit:	NOP
	@MOV	 R0, R4			@ load result to output register
	MOV 	 R7, #1			@ load system call code
	SWI 	 0				@ return this value
	
@ =============================================================================

.data

@ constants about the basic setup of the game: length of sequence and number of colors	
.equ LEN, 3
.equ COL, 3
.equ NAN1, 8
.equ NAN2, 9

@ constants needed to interface with external devices	
.equ BUTTONBASE, 0xFF200050
.equ HEXBASE,	 0xFF200020
.equ BUTTON_NO,  1	

@ you probably want to define a table here, encoding the display of digits on the HEX display	
.align 1	
digits:
	.byte  0b0111111	@ 0
	.byte  0b0000110	@ 1
	.byte  0b1011011	@ 2
	.byte  0b1001111	@ 3

@ INPUT DATA for the matching function
.align 4
secret: .word 1 
	.word 2 
	.word 1 

.align 4
guess:	.word 3 
	.word 1 
	.word 3 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 0 1
.align 4
expect: .byte 0
	.byte 1

.align 4
secret1: .word 1 
	.word 2 
	.word 3 

.align 4
guess1:	.word 1 
	.word 1 
	.word 2 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 1
.align 4
expect1: .byte 1
	.byte 1

.align 4
secret2: .word 2 
	.word 3
	.word 2 

.align 4
guess2:	.word 3 
	.word 3 
	.word 1 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 0
.align 4
expect2: .byte 1
	.byte 0

