/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <malloc.h>
#include <inttypes.h>   // required to print out pointers using PRIXPTR macro
#include "definitions.h"                // SYS function prototypes
#include "testFuncs.h" // lab test structs
#include "printFuncs.h"  // lab print funcs


#define MAX_PRINT_LEN 1000

static uint8_t txBuffer[MAX_PRINT_LEN] = {0};


static char * pass = "PASS";
static char * fail = "FAIL";
static char * oops = "OOPS";

// externs defined in the assembly file:


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */

// returns num elements in array. Returns -1 if exceeds max allowed len,
// or invalid size
static int32_t getLength(void *pResult,int32_t size)
{
    uint8_t *bPtr = (uint8_t *)pResult;    // byte ptr
    uint16_t *hwPtr = (uint16_t *)pResult; // half-word ptr
    uint32_t *wPtr = (uint32_t *)pResult;  // word ptr
    int32_t len = 0;
    if (size == 1)
    {
        while(*bPtr++)
        {
            len += 1;
            if (len > MAX_SORT_LEN)
            {
                len = -1;
                break;
            }
        }
    }
    else if (size == 2)
    {
        while(*hwPtr++)
        {
            len += 1;
            if (len > MAX_SORT_LEN)
            {
                len = -1;
                break;
            }
        }
    }
    else if (size == 4)
    {
        while(*wPtr++)
        {
            len += 1;
            if (len > MAX_SORT_LEN)
            {
                len = -1;
                break;
            }
        }
    }
    else
    {
        len = -1;
    }
    return len;
}


// returns non-zero if there was a failure, whether forced or real
static int32_t check(int32_t in1, 
        int32_t in2, 
        int32_t *goodCount, 
        int32_t *badCount,
        bool forceFail, // if true, forces current test to fail
        char **pfString )
{
    int32_retVal = 0;
    if ((forceFail == true) || (in1 != in2))
    {
        *badCount += 1;
        *pfString = fail;  
        retVal = 1;
    }
    else  //   (in1 == in2)
    {
        *goodCount += 1;
        *pfString = pass;
        retVal = 0;
    }    
    return retVal;
}



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */

static int32_t copyArray(void *inpArr, void *outArr, int32_t elementSize)
{
    int32_t len = getLength(inpArr, outArr, elementSize);
    if (len == -1)
    {
        return -1;
    }
    uint8_t *inp_bPtr = (uint8_t *)inpArr;    // byte ptr
    uint16_t *inp_hwPtr = (uint16_t *)inpArr; // half-word ptr
    uint32_t *inp_wPtr = (uint32_t *)inpArr;  // word ptr
    uint8_t *out_bPtr = (uint8_t *)outArr;    // byte ptr
    uint16_t *out_hwPtr = (uint16_t *)outArr; // half-word ptr
    uint32_t *out_wPtr = (uint32_t *)outArr;  // word ptr
    int32_t len = 0;
    if (elementSize == 1)
    {
        while(*out_bPtr++ = *inp_bPtr++);
    }
    else if (elementSize == 2)
    {
        while(*out_hwPtr++ = *inp_hwPtr++);
    }
    else if (elementSize == 4)
    {
        while(*out_wPtr++ = *inp_wPtr++);
    }
    else
    {
        return -1;
    }
    return 0;
}

// bubble sort. Returns number of swaps.
// assumes length of input is valid, and array is null-terminated
static int32_t sortArray(void *inpArr, int32_t sign, int32_t elementSize)
{
    uint8_t *u_bPtr = (uint8_t *)inpArr;    // byte ptr
    uint16_t *u_hwPtr = (uint16_t *)inpArr; // half-word ptr
    uint32_t *u_wPtr = (uint32_t *)inpArr;  // word ptr
    int8_t *s_bPtr = (int8_t *)inpArr;    // signed byte ptr
    int16_t *s_hwPtr = (int16_t *)inpArr; // signed half-word ptr
    int32_t *s_wPtr = (int32_t *)inpArr;  // signed word ptr
    int32_t numSwaps = 0;
    int32_t totalNumSwaps = 0;
    if ((elementSize == 1) && (sign == 0)) // unsigned bytes
    {
        do
        {
            numSwaps = 0;
            while((*u_bPtr!= 0) &&  (*(u_bPtr+1)!= 0))
            {
                swapped = 0;
                if (*(u_bPtr+1) > *u_bPtr)
                {
                    uint8_t tmp = *(u_bPtr+1);
                    *(u_bPtr+1) = *u_bPtr;
                    *u_bPtr = tmp;
                    numSwaps++;
                }
            }
            totalNumSwaps += numSwaps;
        } while numSwaps != 0;

    }
    else if ((elementSize == 1) && (sign == 1)) // signed bytes
    {
        do
        {
            numSwaps = 0;
            while((*s_bPtr!= 0) &&  (*(s_bPtr+1)!= 0))
            {
                swapped = 0;
                if (*(s_bPtr+1) > *s_bPtr)
                {
                    int8_t tmp = *(s_bPtr+1);
                    *(s_bPtr+1) = *s_bPtr;
                    *s_bPtr = tmp;
                    numSwaps++;
                }
            }
            totalNumSwaps += numSwaps;
        } while numSwaps != 0;
    }
    else if ((elementSize == 2) && (sign == 0)) // unsigned half words
    {
        do
        {
            numSwaps = 0;
            while((*u_hwPtr!= 0) &&  (*(u_hwPtr+1)!= 0))
            {
                swapped = 0;
                if (*(u_hwPtr+1) > *u_hwPtr)
                {
                    uint16_t tmp = *(u_hwPtr+1);
                    *(u_hwPtr+1) = *u_hwPtr;
                    *u_hwPtr = tmp;
                    numSwaps++;
                }
            }
            totalNumSwaps += numSwaps;
        } while numSwaps != 0;

    }
    else if ((elementSize == 2) && (sign == 1)) // signed half words
    {
        do
        {
            numSwaps = 0;
            while((*s_hwPtr!= 0) &&  (*(s_hwPtr+1)!= 0))
            {
                swapped = 0;
                if (*(s_hwPtr+1) > *s_hwPtr)
                {
                    int16_t tmp = *(s_hwPtr+1);
                    *(s_hwPtr+1) = *s_hwPtr;
                    *s_hwPtr = tmp;
                    numSwaps++;
                }
            }
            totalNumSwaps += numSwaps;
        } while numSwaps != 0;
    }

    // words
    
    else if ((elementSize == 4) && (sign == 0)) // unsigned words
    {
        do
        {
            numSwaps = 0;
            while((*u_wPtr!= 0) &&  (*(u_wPtr+1)!= 0))
            {
                swapped = 0;
                if (*(u_wPtr+1) > *u_wPtr)
                {
                    uint32_t tmp = *(u_wPtr+1);
                    *(u_wPtr+1) = *u_wPtr;
                    *u_wPtr = tmp;
                    numSwaps++;
                }
            }
            totalNumSwaps += numSwaps;
        } while numSwaps != 0;

    }
    else if ((elementSize == 4) && (sign == 1)) // signed words
    {
        do
        {
            numSwaps = 0;
            while((*s_wPtr!= 0) &&  (*(s_wPtr+1)!= 0))
            {
                swapped = 0;
                if (*(s_wPtr+1) > *s_wPtr)
                {
                    int32_t tmp = *(s_wPtr+1);
                    *(s_wPtr+1) = *s_wPtr;
                    *s_wPtr = tmp;
                    numSwaps++;
                }
            }
            totalNumSwaps += numSwaps;
        } while numSwaps != 0;
    }
    else
    {
        return -1;
    }
    return numSwaps;
}
// return non-zero if arrays don't match
static int32_t compareArrays(void *expArr,
        void *testArr,
        int32_t elementSize,
        int32_t *goodCount, 
        int32_t *badCount,
        bool forceFail, // if true, forces current test to fail
        char **pfString )
{
    uint8_t *exp_bPtr = (uint8_t *)expArr;    // byte ptr
    uint16_t *exp_hwPtr = (uint16_t *)expArr; // half-word ptr
    uint32_t *exp_wPtr = (uint32_t *)expArr;  // word ptr
    uint8_t *test_bPtr = (uint8_t *)testArr;    // byte ptr
    uint16_t *test_hwPtr = (uint16_t *)testArr; // half-word ptr
    uint32_t *test_wPtr = (uint32_t *)testArr;  // word ptr
    int32_t retVal = 0;
    if (forceFail == true)
    {
        *badCount += 1;
        *pfString = fail;  
        retVal = -1;
    }
    else if (elementSize == 1)  // compare bytes
    {
        while(1)
        {
            // if not equal, signal error and break out of loop
            if (*exp_bPtr != *test_bPtr)
            {
                retVal = 1;
                break;
            }
            if (*exp_bPtr == 0) // if reached the end, break out
            {
                break;
            }
            exp_bPtr += 1;
            test_bPtr += 1;
        } 
    }
    else if (elementSize == 2)  // compare halfwords
    {
        while(1)
        {
            // if not equal, signal error and break out of loop
            if (*exp_bPtr != *test_bPtr)
            {
                retVal = 1;
                break;
            }
            if (*exp_bPtr == 0) // if reached the end, break out
            {
                break;
            }
            exp_bPtr += 1;
            test_bPtr += 1;
        } 
        
    }
    else if (elementSize == 4) // compare words
    {
        while(1)
        {
            // if not equal, signal error and break out of loop
            if (*exp_bPtr != *test_bPtr)
            {
                retVal = 1;
                break;
            }
            if (*exp_bPtr == 0) // if reached the end, break out
            {
                break;
            }
            exp_bPtr += 1;
            test_bPtr += 1;
        } 
        
    }
    else
    {
        // something is wrong
        retVal = -2;
    }
    
    if (retVal != 0)
    {
        *badCount += 1;
        *pfString = fail;  
    }
    else 
    {
        *goodCount += 1;
        *pfString = pass;
    }    
    
    return retVal;
}



// return -1 if invalid input
static int32_t calcExpectedValues(
        void *inpArr, // unsorted input array from caller
        void *outArr, // pointer where caller wants sorted output to be stored
        int32_t sign, // input: 1 == signed, 0 == unsigned
        int32_t size, // input: 1,2, or 4 byte elements
        expectedValues *e) // in/out: ptr to struct where values will be stored
{
    // copy input to output
    
    // sort the values in the output array
    
    return 0;
}



void testResult(int testNum, 
                void *inpArr, // copy of unsorted input array sent to asm
                void * expectedResultAddr, // pointer to sorted output arr from asm
                void * pResult, // ptr returned by asm
                int32_t sign, // whether inp arr was signed or unsigned
                int32_t size, // num butes for each element in array: 1,2, or 4
                void *pGood, //ptr where asm was told to get input and store output
                int32_t* passCnt,
                int32_t* failCnt,
                volatile bool * txComplete)
{    
    expectedValues e;

    // place to store pass/fail strings
    char * ptrCheck = oops;
    char * lenCheck = oops;
    char * sortCheck = oops;

    *passCnt = 0;
    *failCnt = 0;
    int32_t err = calcExpectedValues(
            (void *) inpArr,
            (void *) sortedInpArr, 
            sign,
            size, 
            &e);
    
    // set force fail flag to false. If certain tests fail, 
    // remaining tests are forced to fail
    bool forceFail = false;
    int32_t err = 0;
    
    // test that the pointer returned by asmSort points to value passed to asm
    err = check((int32_t)expectedResultAddr,(int32_t)pResult,forceFail,passCnt,failCnt,&ptrCheck);
    if (err != 0) // force remaining tests to fail cuz not sure where sorted list was written
    {
        forceFail = true;
    }
    
    // test to see if sorted array len matches expectations
    int32_t asmLength = -1;
    if (forceFail == false)
    {
        // return num elements excluding trailing 0. 
        // -1 if exceeds max allowable length
        asmLength = getLength(pResult,size); // return num elements excluding trailing 0
    }
    err = check(e.expectedLen,asmLength,passCnt,failCnt,forceFail,&lenCheck );
    // force remaining tests to fail cuz can't compare different len arrays
    if (err != 0) 
    {
        forceFail = true;
    }
    
    compareArrays(&e.expectedSortedArr[0],
            pResult,
            passCnt,
            failCnt,
            forceFail,
            &sortCheck );
      
    snprintf((char*)txBuffer, MAX_PRINT_LEN,
            "========= Test Number: %d\r\n"
            "%s: pointer check: expected:  0x%" PRIXPTR "; actual: 0x%" PRIXPTR "\r\n"
            "%s: length expected: %ld; actual: %ld\r\n"
            "%s: Result of sorted list comparison\r\n"
            "tests passed: %ld; tests failed: %ld \r\n"
            "\r\n",
            testNum,
            ptrCheck,(uintptr_t)(&expectedResultAddr), (uintptr_t)pResult,
            lenCheck,e.expectedLen,asmLength,
            sortCheck,
            *passCnt,*failCnt); 
    
    printAndWait((char*)txBuffer,txComplete);
    
    return;
}




/* *****************************************************************************
 End of File
 */
