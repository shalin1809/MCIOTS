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
******************************************************************************
* emlib library of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
*
* The functions that use this library are:
* 1. void LETIMER_Setup(void);
* 2. void LETIMER0_CAL_SETUP(void);
* 3. void LETIMER0_IRQHandler(void);
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
* Global variables: CurrentLFAFreq
*
* Returned variables: None
**************************************************************************/
void LETIMER_Setup(void)
{
    float Period, OnTime;                       //Variables for Period and Ontime

    const LETIMER_Init_TypeDef letimerInit =
    {
        .enable         = false,                //Start counting when init completed
        .debugRun       = false,                //Counter shall not keep running during debug halt
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

    //CMU->LFAPRESC0 = CMU_LFAPRESC0_LETIMER0_DIV1;
    CurrentLFAFreq = CMU_ClockFreqGet(cmuClock_LFA);            //Get current frequency of LFA clock tree
    if(CMU_ClockSelectGet(cmuClock_LFA) == cmuSelect_ULFRCO)    //If ULFRCO is the current clock in use
    {
        Period = (CurrentLFAFreq * LETIMER_PERIOD);
        Period *= clock_scale;                                  //Calculate period with the scaling factor for LETIMER0
        OnTime = (CurrentLFAFreq * LETIMER_ONTIME);
        OnTime *= clock_scale;                                  //Calculate on time with the scaling factor for LETIMER0
    }
    else
    {
        Period = (CurrentLFAFreq * LETIMER_PERIOD);             //Else, calculate period for LETIMER0
        OnTime = (CurrentLFAFreq * LETIMER_ONTIME);             //Calculate Ontime for the LETIMER0
    }


    LETIMER0_Prescaler = 0;                                     //Initialize prescaler to 0
    while(Period > LETIMER_MAX_COUNT)                           //Till the period exceeds the max value of LETIMER counter
    {
        LETIMER0_Prescaler++;                                   //Increment the value of prescaler
        Period/=2;                                              //Divide period by the power of 2
        OnTime/=2;                                              //Divide on time by the power of 2
    }
    CMU->LFAPRESC0 = (LETIMER0_Prescaler<<8);                   //Set the LFA LETIMER prescaler value with the prescaler value
    LETIMER_CompareSet(LETIMER0, 0, Period);                    //Set the period to COMP0, this will be the top of the timer
    LETIMER_CompareSet(LETIMER0, 1, OnTime);                    //Set Ontime to COMP1


    LETIMER_Init(LETIMER0, &letimerInit);                       //Initilize LETIMER0
    while(LETIMER0->SYNCBUSY);                                  //Wait till SYNCBUSY is cleared

    LETIMER0->CNT = Period;                                     //Initialize counter same as COMP0
    LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1); //Clear all interrupts
    LETIMER_IntEnable(LETIMER0 , LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);    //Enable interrupts for COMP0 and COMP1
    NVIC_EnableIRQ(LETIMER0_IRQn);                              //Enable global interrupts for LETIMER0
    BlockSleepMode(MinimumEnergyMode);                          //Set the minimum Block Sleep mode
}



/************************************************************************
* Setup and initialize LETIMER0 for calibration
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void LETIMER0_CAL_SETUP(void)
{
    /*structure defining the configurations of LETIMER0*/
    const LETIMER_Init_TypeDef letimer_cal_Init =
    {
      .enable         = false,                  // Don't start counting when init completed.
      .debugRun       = false,                  // Counter shall not keep running during debug halt.
      .rtcComp0Enable = false,                  // Don't start counting on RTC COMP0 match.
      .rtcComp1Enable = false,                  // Don't start counting on RTC COMP1 match.
      .comp0Top       = false,                  // Don't load COMP0 register into CNT when counter underflows. COMP0 is used as TOP
      .bufTop         = false,                  // Don't load COMP1 into COMP0 when REP0 reaches 0.
      .out0Pol        = 0,                      // Idle value for output 0.
      .out1Pol        = 0,                      // Idle value for output 1.
      .ufoa0          = letimerUFOANone,        // No changes on underflow
      .ufoa1          = letimerUFOANone,        // No changes on underflow
      .repMode        = letimerRepeatOneshot    // will run for only one time
    };
    LETIMER_Init(LETIMER0, &letimer_cal_Init);  // Initialize LETIMER0
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
    __disable_irq();                        //Disable interrupts to avoid nesting
    intFlags = LETIMER_IntGet(LETIMER0);    //Get the interrupting flags
    if(intFlags & LETIMER_IF_COMP0)         //If COMP0 interrupts, turn off the LED
    {
        ACMPstatus = (ACMP0->STATUS & ACMP_STATUS_ACMPOUT);                 //Read ACMP output
        GPIO_PinOutClear(LIGHTSENSE_EXCITE_PORT, LIGHTSENSE_EXCITE_PIN);    //Turn off light sensor excitation
        ACMP0->CTRL &= ~ACMP_CTRL_EN;       //Disable ACMP
        LETIMER0->IFC = intFlags;           //Clear interrupt flags
        if(ACMPstatus)                                      //if output is high
        {
            if(acmpInit.vddLevel == LOW_THRESHOLD)          //if the current reading is for low threshold
            {
                acmpInit.vddLevel = HIGH_THRESHOLD;         //change threshold to high
                ACMP_Init(ACMP0, &acmpInit);                //re-initialize ACMP with new VddLevel
                ACMP_ChannelSet(ACMP0, acmpChannelVDD, acmpChannel6);   //Swap positive and negative inputs of ACMP0
                ledON(0);                                   //Turn ON LED
            }
            else                                            //if current reading is for high threshold
            {
                acmpInit.vddLevel = LOW_THRESHOLD;          //change threshold to low
                ACMP_Init(ACMP0, &acmpInit);                //re-initialize ACMP with new VddLevel
                ACMP_ChannelSet(ACMP0, acmpChannel6, acmpChannelVDD);   //Swap positive and negative inputs of ACMP0
                ledOFF(0);                                  //Turn off LED
            }

        }
    }
    else                                                                    //If COMP1 interrupts
    {
        ACMP0->CTRL |= ACMP_CTRL_EN;                                        //Enable ACMP0 warm-up
        GPIO_PinOutSet(LIGHTSENSE_EXCITE_PORT, LIGHTSENSE_EXCITE_PIN);      //Excite Ambient Light Sensor
        LETIMER0->IFC = intFlags;                                           //Clear interrupt flags
        while(!(ACMP0->STATUS & ACMP_STATUS_ACMPACT));                      //Wait for warm-up to finish
    }

    __enable_irq();                         //Re-enable interrupts
}



