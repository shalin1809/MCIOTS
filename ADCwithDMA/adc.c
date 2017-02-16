/******************************************************************************
* File: adc.c
*
* Created on: 10-Feb-2017
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
* 1. void Init_ADC0(void);
******************************************************************************/

/*****************************************************
            * Include Statements *
 *****************************************************/
#include "adc.h"




/************************************************************************
* Initialize the ADC0 for single mode
*
* Input variables: None
*
* Global variabprsEnableles: None
*
* Returned variables: None
*
**************************************************************************/
void Init_ADC0(void)
{
    ADC_Init_TypeDef ADC0Init =
    {
        .ovsRateSel = ADC_CTRL_OVSRSEL_DEFAULT,             //Set oversampling to default, no effect as oversampling is not used
        .lpfMode = ADC_CTRL_LPFMODE_BYPASS,                 //Low pass filter set to bypass
        .warmUpMode = ADC_CTRL_WARMUPMODE_FASTBG,           //Bandgap ref does not need to warm up
        .timebase = ADC_TIMEBASE,                           //Warmup time for the ADC
        .prescale = ADC_Prescale,                           //Prescalar to be used
        .tailgate = false                                   //Disable conversion tailgating
    };

    ADC_Init(ADC0, &ADC0Init);                              //Initialize ADC0

    ADC_InitSingle_TypeDef ADC0_InitSingle =
    {
        .prsSel = ADC_SINGLECTRL_PRSEN_DEFAULT,             //Default valur for PRS trigger
        .prsEnable = false,                                 //Disable PRS trigger
        .acqTime = ADC_ACQ_2CYCLES,                         //Acquisition time set to 2 cycles
        .reference = ADC_TEMP_REF,                          //Internal 1.25V reference selected
        .input = ADC_TEMP_SENSOR,                           //Temperature sensor selected as input
        .resolution = ADC_RES_12BIT,                        //12-bit resolution
        .leftAdjust = false,                                //Default right aligned
        .diff = false,                                      //Single ended input
        .rep = true                                         //Continuous repetitive mode
    };

    ADC_InitSingle(ADC0, &ADC0_InitSingle);                 //Initialize ADC0 for singlemode
    cal_temp_0 = ((DEVINFO->CAL & _DEVINFO_CAL_TEMP_MASK) >> _DEVINFO_CAL_TEMP_SHIFT);  //Device temperature calibration value
    cal_value_0 = ((DEVINFO->ADC0CAL2 & _DEVINFO_ADC0CAL2_TEMP1V25_MASK) >> _DEVINFO_ADC0CAL2_TEMP1V25_SHIFT);  //Device ADC calibration value
    NVIC_EnableIRQ(ADC0_IRQn);                              //Enable NVIC interrupts
}

/************************************************************************
* Interrupt Handler to the ADC
*
* Input variables: None
*
* Global variables: adcSum, count
*
* Returned variables: temperature
*
**************************************************************************/
void ADC0_IRQHandler(void)
{
    __disable_irq();
    ADC0->IFC |= ADC_IFC_SINGLE | ADC_IFC_SINGLEOF; //Clear interrupt flags
    --count;                                        //Decrement the counter
    adcSum += ADC0->SINGLEDATA;                     //Add the ADC reading to the sum
    if(count==0)                                    //If the required number of ADC samples are recorded
    {
        ADC0->IEN &= ~ADC_IEN_SINGLE;               //Disable interrupts
        ADC0->CMD |= ADC_CMD_SINGLESTOP;            //Stop ADC0
        UnblockSleepMode(ADC_EM);                   //Unblock minimum sleep mode required for ADc
        float temp = convertToCelcius();            //Calculate the temperature
        if(TEMP_LOW_THRESHOLD > temp || temp > TEMP_HIGH_THRESHOLD) //If the temperature goes beyond the boundaries
        {
            ledON(1);                               //Turn on LED 1
        }
        else                                        //If it is within limits
            ledOFF(1);                              //Turn OFF LED 1
    }
    __enable_irq();
}

