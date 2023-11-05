/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project. It is intended to
    be used as the starting point for CISC-211 Curiosity Nano Board
    programming projects. After initializing the hardware, it will
    go into a 0.5s loop that calls an assembly function specified in a separate
    .s file. It will print the iteration number and the result of the assembly 
    function call to the serial port.
    As an added bonus, it will toggle the LED on each iteration
    to provide feedback that the code is actually running.
  
    NOTE: PC serial port should be set to 115200 rate.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

//DOM-IGNORE-BEGIN 
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <float.h>
#include "definitions.h"                // SYS function prototypes
#include "printFuncs.h"  // lab print funcs
#include "testFuncs.h"  // lab print funcs

/* RTC Time period match values for input clock of 1 KHz */
#define PERIOD_50MS                             51
#define PERIOD_500MS                            512
#define PERIOD_1S                               1024
#define PERIOD_2S                               2048
#define PERIOD_4S                               4096

#define MAX_PRINT_LEN 1000



static volatile bool isRTCExpired = false;
static volatile bool changeTempSamplingRate = false;
static volatile bool isUSARTTxComplete = true;
static uint8_t uartTxBuffer[MAX_PRINT_LEN] = {0};

// static char * pass = "PASS";
// static char * fail = "FAIL";
// static char * oops = "OOPS";

// PROF COMMENT:
// The ARM calling convention permits the use of up to 4 registers, r0-r3
// to pass data into a function. Only one value can be returned from a C
// function call. It will be stored in r0, which will be automatically
// used by the C compiler as the function's return value.
//
// Function signature
// For this lab, return the larger of the two floating point values passed in.
extern int32_t asmSwap(void *, int32_t sign, int32_t elemSize);
extern int32_t asmSort(void *, int32_t sign, int32_t elemSize);

// externs defined in the assembly file:
extern float f1,f2,fMax;
extern uint32_t sb1,sb2,signBitMax;
// ERROR: exp2 is the name of a built-in C function!
// extern int32_t exp1,exp2,expMax; // adjusted UNBIASED exponent
extern int32_t expMax; // adjusted UNBIASED exponent
extern uint32_t mant1,mant2,mantMax; // adjusted mantissa (hidden bit added when appropriate))
extern int32_t biasedExp1,biasedExp2,biasedExpMax;
extern uint32_t nanValue;


// have to play games with data types to get floats to be passed in r0 and r1
// otherwise, assy needs to use VMOV instructions to move from s registers
// to r registers
static uint32_t reinterpret_float(float f)
{
    float *pf = &f;
    void * pv = (void *) pf;
    uint32_t * pi = (uint32_t *) pv;
    return *pi;
}

static uint8_t u8TestCases[][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {1,2,3,4,5,6,7,8,9,0}
}

static uint16_t u16TestCases[][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {1,2,3,4,5,6,7,8,9,0}
}

static uint32_t u32TestCases[][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {1,2,3,4,5,6,7,8,9,0}    
}
#define USING_HW 1

#if USING_HW
static void rtcEventHandler (RTC_TIMER32_INT_MASK intCause, uintptr_t context)
{
    if (intCause & RTC_MODE0_INTENSET_CMP0_Msk)
    {            
        isRTCExpired    = true;
    }
}
static void usartDmaChannelHandler(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        isUSARTTxComplete = true;
    }
}
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    
 
#if USING_HW
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, usartDmaChannelHandler, 0);
    RTC_Timer32CallbackRegister(rtcEventHandler, 0);
    RTC_Timer32Compare0Set(PERIOD_50MS);
    RTC_Timer32CounterSet(0);
    RTC_Timer32Start();

#else // using the simulator
    isRTCExpired = true;
    isUSARTTxComplete = true;
#endif //SIMULATOR

    int32_t passCount = 0;
    int32_t failCount = 0;
    int32_t totalPassCount = 0;
    int32_t totalFailCount = 0;
    int32_t totalTestCount = 0;
    int32_t numTestCases = 0;
    
    while ( true )
    {
        int32_t swapTotalTestCount = 0;
        int32_t swapTotalPassCount = 0;
        int32_t swapTotalFailCount = 0;
        numTestCases = sizeof(swapTestCases)/sizeof(swapTestCasses[0]);
        for(int testCase = 0; testCase<numTestCases; ++testCase)
        {
            LED0_Toggle();
            // reset the state variables for the timer and serial port funcs
            isRTCExpired = false;
            isUSARTTxComplete = false;
            
            // reset the counters for this test pass
            passCount = 0;
            failCount = 0;
            
            // Get the value for this test case
            
            // call the func
            
            // test the result
            
            // print the result
            
            // wait for the toggle timer to expire
            while(isRTCExpired == false);
        } // end: for (testCase = 0; ...)
        
        //OLD CODE!!!!!!!!!!!!!
        if (isRTCExpired == true)
        {
            isRTCExpired = false;
            
            LED0_Toggle();
            
            // copy test case values to a new array
            // TODO
            
            // create a sorted list for comparison
            // TODO
            
            void * returnPtr = 0;
            // call the asm function to do the sort
            returnPtr = asmSort((void *)&u8TestCases[0][0],0,1);
            
            // TODO test the returned values
            testResult(iteration,tc[iteration][0],tc[iteration][1],
                    max,
                    &fMax,
                    &passCount,
                    &failCount,
                    &isUSARTTxComplete);
            totalPassCount += passCount;        
            totalFailCount += failCount;        
            totalTestCount += failCount + passCount;        
             ++iteration;
            
            if (iteration >= maxIterations)
            {
                break; // tally the results and end program
            }
            
        }
        /* Maintain state machines of all polled MPLAB Harmony modules. */
    }

#if USING_HW
    int32_t NUM_POINTS_AVAILABLE = 20
    snprintf((char*)uartTxBuffer, MAX_PRINT_LEN,
            "========= ALL TESTS COMPLETE!\r\n"
            "tests passed: %ld \r\n"
            "tests failed: %ld \r\n"
            "total tests:  %ld \r\n"
            "score: %ld/20 points \r\n\r\n",
            totalPassCount,
            totalFailCount,
            totalTestCount,
            NUM_POINTS_AVAILABLE*totalPassCount/totalTestCount); 
            
    isUSARTTxComplete = false;
    
    printAndWait((char*)uartTxBuffer,&isUSARTTxComplete);

#else
            isRTCExpired = true;
            isUSARTTxComplete = true;
            if (iteration >= maxIterations)
            {
                break; // end program
            }

            continue;
#endif

    
    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}
/*******************************************************************************
 End of File
*/

