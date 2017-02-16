/******************************************************************************
* File: timer.c
*
* Created on: 07-Feb-2017
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
******************************************************************************
******************************************************************************
* emlib library of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
*
* The functions that use this library are:
* 1. void TIMER0_SETUP(void);
* 2. void TIMER1_SETUP(void);
******************************************************************************/


/*****************************************************
            * Include Statements *
 *****************************************************/
#include "timer.h"


/************************************************************************
* Initialize Timer 0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
void TIMER0_SETUP(void)
{
    /* Creating struct to set the TIMER0 parameters */
    TIMER_Init_TypeDef timer0Init =
      {
        .enable     = true,                             //Start counting when init is completed
        .debugRun   = false,                            //Counter stops counting during debug
        .prescale   = timerPrescale1,                   //No prescaler required
        .mode       = timerModeUp,                      //Timer to be used in up counter mode
        .dmaClrAct  = false,                            //No DMA clear request
        .quadModeX4 = false,                            //No quad mode used
        .clkSel     = timerClkSelHFPerClk,              //Select clock to High frequency
        .fallAction = timerInputActionNone,             //No action on falling input edge
        .riseAction = timerInputActionNone,             //No action on rising input edge
        .oneShot    = false,                            //Continuous counting.
        .sync       = true,                             //In synchronization with other timers
      };

    TIMER_Init(TIMER0, &timer0Init);                    //Initializing TIMER0
}


/************************************************************************
* Initialize Timer 1
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
void TIMER1_SETUP(void)
{
    /* Creating struct to set the TIMER1 parameters */
    TIMER_Init_TypeDef timer1Init =
    {
        .enable     = true,                             //Start counting when init is completed
        .debugRun   = false,                            //Counter stops counting during debug
        .prescale   = timerPrescale1,                   //No prescaler required
        .mode       = timerModeUp,                      //Timer to be used in up counter mode
        .dmaClrAct  = false,                            //No DMA clear request
        .quadModeX4 = false,                            //No quad mode used
        .clkSel     = timerClkSelCascade,               //Select clock to Cascade mode. Clock gets triggered on UF/OF of TIMER0
        .fallAction = timerInputActionNone,             //No action on falling input edge
        .riseAction = timerInputActionNone,             //No action on rising input edge
        .oneShot    = false,                            //Continuous counting.
        .sync       = true,                             //In synchronization with other timers
    };

    TIMER_Init(TIMER1, &timer1Init);                    //Initializing TIMER1
}
