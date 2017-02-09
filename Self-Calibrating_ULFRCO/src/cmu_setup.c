/******************************************************************************
* File: cmu_setup.c
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
******************************************************************************
* emlib library of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
*
* The functions that use this library are:
* 1. void CMU_Setup(void);
* 2. void CMU_CAL_ULFRCO(uint32_t milliseconds);
******************************************************************************/


/*****************************************************
            * Include Statements *
 *****************************************************/
#include "cmu_setup.h"


/************************************************************************
* Setup and initialize the Clock Management Unit
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void CMU_Setup(void)
{
    if(MinimumEnergyMode == EM3)
    {
        CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);    //Enable ULFRCO clock for EM3
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); //Select ULFRCO as the clock for LFA clock tree to the letimer
    }
    else
    {
        CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      //Enable LXF0 clock oscillator
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   //Select LFXO as the clock for LFA clock tree to the letimer
    }
    CMU_ClockEnable(cmuClock_CORELE, true);                 //Enable clock to Low energy peripheral clock tree
    CMU_ClockEnable(cmuClock_LETIMER0, true);               //Enable clock to letimer0
    CMU_ClockEnable(cmuClock_ACMP0, true);                  //Enable clock to ACMP0
}



/************************************************************************
* Setup and initialize the Clock Management Unit for calibrating ULFRCO
*
* Input variables: milliseconds
*
* Global variables: clock_scale, total_count_LFX0, total_count_ULFRCO
*
* Returned variables: None
**************************************************************************/
void CMU_CAL_ULFRCO(uint32_t milliseconds)
{
    CMU_ClockEnable(cmuClock_HFPER,true);               //Enable 14 MHz High Frequency RC clock
    CMU_ClockEnable(cmuClock_TIMER0,true);              //Enable clock to TIMER0
    CMU_ClockEnable(cmuClock_TIMER1,true);              //Enable clock to TIMER1
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      //Enable LXF0 clock oscillator
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   //Select LFXO as the clock for LFA clock tree to the letimer
    CMU_ClockEnable(cmuClock_CORELE, true);             //Enable clock to Low energy peripheral clock tree
    CMU_ClockEnable(cmuClock_LETIMER0, true);           //Enable clock to letimer0
    TIMER0_SETUP();                                     //Initialize Timer0 as up counter
    TIMER1_SETUP();                                     //Initialize Timer1 cascaded to timer0 to work as a 32-bit timer
    LETIMER0_CAL_SETUP();                               //Initialize LETIMER0 in one shot mode for calibration

    CurrentLFAFreq = CMU_ClockFreqGet(cmuClock_LFA);    //Get the current frequency of LFA clock tree
    float count = (CurrentLFAFreq * milliseconds / 1000);     //Calculate the counter value for required time
    LETIMER0->CNT = (uint16_t)count;                    //Set the counter in LETIMER0
    TIMER1->CNT = 0;                                    //Initialize Timer1 counter to 0
    TIMER0->CNT = 0;                                    //Initialize Timer0 counter to 0
    LETIMER_Enable(LETIMER0, true);                     //Enable LETIMER0, Timer 0 and 1 will start in sync


    while((LETIMER0->CNT)!=0);                          //Wait till one shot of letimer is complete
    total_count_LFXO = TIMER0->CNT;                     //Read the lower word of the 32 bit timer
    total_count_LFXO |= (TIMER1->CNT << 16);            //Read the higher word of the 32 bit timer

    CMU_OscillatorEnable(cmuOsc_LFXO, false, true);     //disable LXF0 clock oscillator
    LETIMER_Reset(LETIMER0);                            //disable LETIMER0
    CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);    //Enable ULFRCO clock
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); //Select ULFRCO as the clock for LFA clock tree to the letimer
    LETIMER0_CAL_SETUP();                               //Initialize LETIMER0 in one shot mode for calibration

    CurrentLFAFreq = CMU_ClockFreqGet(cmuClock_LFA);    //Get current frequency of LFA clock tree after changing the clock
    count = (CurrentLFAFreq * milliseconds / 1000);     //Calculate the counter value for required time
    LETIMER0->CNT = (uint16_t)count;                    //Set the counter in LETIMER0
    TIMER1->CNT =0;                                     //Initialize Timer1 counter to 0
    TIMER0->CNT =0;                                     //Initialize Timer0 counter to 0
    LETIMER_Enable(LETIMER0, true);                     //Enable LETIMER0, Timer 0 and 1 will start in sync

    while((LETIMER0->CNT)!=0);                          //Wait till one shot of letimer is complete
    total_count_ULFRCO = TIMER0->CNT;                   //Read the lower word of the 32 bit timer
    total_count_ULFRCO |= (TIMER1->CNT << 16);          //Read the higher word of the 32 bit timer

    CMU_ClockEnable(cmuClock_TIMER0,false);             //disable clock to TIMER0
    CMU_ClockEnable(cmuClock_TIMER1,false);             //disable clock to TIMER1
    CMU_OscillatorEnable(cmuOsc_ULFRCO, false, true);   //disable ULFRCO clock
    CMU_ClockEnable(cmuClock_CORELE, false);            //disable clock to Low energy peripheral clock tree
    CMU_ClockEnable(cmuClock_LETIMER0, false);          //disable clock to letimer0
    clock_scale = (float)total_count_LFXO/(float)total_count_ULFRCO;    //Calculate the clock scaling factor required for calibration
}

