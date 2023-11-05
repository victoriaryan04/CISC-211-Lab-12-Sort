/*** asmSort.s   ***/
#include <xc.h>
.syntax unified

@ Declare the following to be in data memory
.data
.align    

@ Define the globals so that the C code can access them

.if 0    
@ left these in as an example. Not used.
.global fMax
.type fMax,%gnu_unique_object
 fMax: .word 0
.endif 

@ Tell the assembler that what follows is in instruction memory    
.text
.align

/********************************************************************
function name: asmSwap(inpAddr,signed,size)
function description:
    Checks magnitude of each of two input values 
    v1 and v2 that are adjacent in memory.
    v1 is located in memory location (inpAddr)
    v2 is located at mem location (inpAddr + size)
    
    If v1 or v2 is 0, this function immediately
    places 0 in r0 and returns to the caller.
    
    Else, if v1 is greater than v2, this function 
    swaps them. 

    If values were swapped, the function returns any non-zero 
    value in r0. (It could be either of the two 
    mem values, or anything else as long as it's not 0.)
    
    If values were NOT swapped, return 0 in r0.
         
Inputs: r0: inpAddr: address of v1 to be examined. 
	             Address of v2 is: 
                     inpAddr + size
	r1: signed: 1 indicates values are signed, 
	            0 indicates values are unsigned
	r2: size: number of bytes for each input value.
                  Valid values: 1, 2, 4
Outputs: r0: If either input value is 0, return -1 in r0
             If neither input value is 0, and a swap was
             made, return 1.
             If neither input value is 0, and a swap was
             NOT made, return 0.
             
         Memory: if v2>v1, swap v1 and v2.
                 Else, if v1 or v2 is 0, OR if v1 <= v2
                 do not modify values in memory.
NOTE: definitions: "greater than" means most positive number
********************************************************************/     
.global asmSwap
.type asmSwap,%function     
asmSwap:

    
/********************************************************************
function name: admSort(startAddr,signed,size)
function description:
    Sorts value in an array from lowest to highest.
    The end of the input array is marked by a value
    of 0.
    The values are sorted "in-place" (i.e. upon returning
    to the caller, the first element of the sorted array 
    is located at startAddr)
    The function returns the ttal number of swaps that were
    required in r0. 
    
         
Inputs: r0: startAddr: address of first value in unsorted 
                       array.
		       Second element will be located at:
                       inpAddr + size
	r1: signed: 1 indicates values are signed, 
	            0 indicates values are unsigned
	r2: size: number of bytes for each input value.
                  Valid values: 1, 2, 4
Outputs: r0: number of swaps required to sort the array
         Memory: The original input values will be
                 sorted and stored in memory starting
		 at mem location startAddr
NOTE: definitions: "greater than" means most positive number
********************************************************************/     
.global asmSort
.type asmSort,%function
asmSort:   

    /* Note to Profs: Solution used to test c code is located in Canvas:
     *    Files -> Lab Files and Coding Examples -> Lab 11 Float Solution
     */

    /* YOUR asmSort CODE BELOW THIS LINE! VVVVVVVVVVVVVVVVVVVVV  */
    
    
    /* YOUR asmSort CODE ABOVE THIS LINE! ^^^^^^^^^^^^^^^^^^^^^  */

   

/**********************************************************************/   
.end  /* The assembler will not process anything after this directive!!! */
           




