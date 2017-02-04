/******************************************************************************
* File: letimer.c
*
* Created on: 04-Feb-2017
* Author: Shalin Shah
* 
*******************************************************************************
* @section License
* <b>Copyright 2016 Silicon Labs, Inc. http://www.silabs.com</b>
*******************************************************************************
*
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
* obligation to support this Software. Silicon Labs is providing the
* Software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Silicon Labs will not be liable for any consequential, incidental, or
* special damages, or any other relief, or for any claim by any third party,
* arising from your use of this Software.
******************************************************************************/


/*****************************************************
            * Include Statements *
 *****************************************************/
#include "letimer.h"





/************************************************************************
* Setup and initialize the LETIMER
*
* Input variables: None
*
* Global variables: CurrentClockFreq
*
* Returned variables: None
**************************************************************************/
void LETIMER_Setup(void)
{
    int Period, OnTime;                         //Variables for Period and Ontime
    const LETIMER_Init_TypeDef letimerInit =
    {
        .enable         = true,                 //Start counting when init completed
        .debugRun       = true,                 //Counter shall not keep running during debug halt
        .rtcComp0Enable = false,                //Don't start counting on RTC COMP0 match
        .rtcComp1Enable = false,                //Don't start counting on RTC COMP1 match
        .comp0Top       = true,                 //Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP
        .bufTop         = false,                //Don't load COMP1 into COMP0 when REP0 reaches 0
        .out0Pol        = 0,                    //Idle value for output 0.
        .out1Pol        = 0,                    //Idle value for output 1.
        .ufoa0          = letimerUFOANone,      //PWM output on output 0
        .ufoa1          = letimerUFOANone,      //Pulse output on output 1
        .repMode        = letimerRepeatFree     //Count until stopped
    };

    CurrentClockFreq = CMU_ClockFreqGet(cmuClock_LFA);
    Period = (CurrentClockFreq * LEDPeriod);
    LETIMER_CompareSet(LETIMER0, 0, Period);
    OnTime = (CurrentClockFreq * LEDOnTime);
    LETIMER_CompareSet(LETIMER0, 1, OnTime);

    LETIMER_Init(LETIMER0, &letimerInit);
    while(LETIMER0->SYNCBUSY);

    LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1);
    LETIMER_IntEnable(LETIMER0 , LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);
    NVIC_EnableIRQ(LETIMER0_IRQn);
    BlockSleepMode(MinimumEnergyMode);
}

/************************************************************************
* Interrupt handler routine for LETIMER0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void LETIMER0_IRQHandler(void)
{
    static int intFlags;                    //Variable to store the interrupting flags
    __disable_irq();                        //Diable interrupts to avoid nesting
    intFlags = LETIMER_IntGet(LETIMER0);    //Get the interrupting flags
    LETIMER_IntClear(LETIMER0, intFlags);   //Clear the interrupting flags
    if(intFlags & LETIMER_IF_COMP0){        //If COMP0 interrupts, turn off the LED
        ledOFF(0);
    }
    else
    {
        ledON(0);                           //COMP1 interrupts, turn on LED

    }
    __enable_irq();                         //Re-enable interrupts
}

