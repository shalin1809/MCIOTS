/******************************************************************************
* File: adc.h
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

#ifndef ADC_H
#define ADC_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_adc.h"
#include "sleep.h"
#include "em_device.h"                      //It includes the DEVINFO required for calibration
#include "led_control.h"
#include "leuart.h"


/*****************************************************
 			* Define Statements *
 *****************************************************/
#define ADC_TEMP_REF    _ADC_SINGLECTRL_REF_1V25            //ADC reference used for temperature sensor
#define ADC_TEMP_SENSOR _ADC_SINGLECTRL_INPUTSEL_TEMP       //ADC input source for temperature sensor
#define ADC_ACQ_2CYCLES _ADC_SINGLECTRL_AT_2CYCLES          //ADC acquisition set to 2 cycles
#define ADC_ACQ_4CYCLES _ADC_SINGLECTRL_AT_4CYCLES          //ADC acquisition set to 4 cycles
#define ADC_RES_12BIT   _ADC_SINGLECTRL_RES_12BIT           //ADC resolution set to 12 bit
#define ADC_Prescale    9                                   //Prescalar value to prescale by 50
#define TGRAD_ADC       -6.3                                //Temperature gradient given in the datasheet
#define NUMBER_OF_ADC_SAMPLES 500                           //Number of ADC samples to be averaged
#define TGRAD_ADCTH (TGRAD_ADC*NUMBER_OF_ADC_SAMPLES)       //Temperature gradient for required number of cycles
#define ADC_TIMEBASE    14                                  //Timebase required for 1uS
#define ADC_EM          EM1                                 //Minimum Energy mode for ADC



#define USE_DMA             true                            //Use DMA for ADC transfers, false will use ADC IRQ
#define TEMP_LOW_THRESHOLD  15                              //Lower threshold temperature value
#define TEMP_HIGH_THRESHOLD 35                              //Higher threshold temperature value

/*****************************************************
            * Global Variables *
 *****************************************************/
uint32_t adcSum;                                            //To store the sum of the ADC samples
uint32_t cal_temp_0;                                        //To store the device temperature calibration
uint32_t cal_value_0;                                       //To store device ADC calibration value
unsigned int count;                                         //Counter for ADC samples


/************************************************************************
* Initialize the ADC0 for single mode
*
* Input variables: None
*
* Global variables: cal_temp_0, cal_value_0
*
* Returned variables: None
*
**************************************************************************/
void Init_ADC0(void);


/************************************************************************
* Start ADC to read using interrupts
*
* Input variables: None
*
* Global variables: count, adcSum
*
* Returned variables: None
*
**************************************************************************/
__STATIC_INLINE void ADC_Read(void)
{
    count = NUMBER_OF_ADC_SAMPLES;          //Initialize the counter to number of cycles required
    adcSum = 0;                             //Initialize the sum to 0
    BlockSleepMode(ADC_EM);                 //Block EM1 so ADC keeps functioning
    ADC0->IEN |= ADC_IEN_SINGLE;            //Enable interrupts for ADC
    ADC0->CMD |= ADC_CMD_SINGLESTART;       //Start ADC0
}



/************************************************************************
* Convert the ADC samples to celsius
*
* Input variables: None
*
* Global variables: adcSum, cal_temp_0, cal_value_0
*
* Returned variables: temperature
*
* Code Algorithm source: Silicon Labs and ECEN 5023-001 Lecture7.pdf by Keith Graham
**************************************************************************/
__STATIC_INLINE float convertToCelcius(void)
{
    float temperature = 0;                              //Initialize temperature to 0
    /* Calculate the temperature using the device calibration values */
    temperature = cal_value_0*NUMBER_OF_ADC_SAMPLES;
    temperature -= adcSum;
    temperature /= TGRAD_ADCTH;
    temperature = (float)cal_temp_0 - temperature;
    return temperature;                                 //return the calculated temperature value
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
void ADC0_IRQHandler(void);

#endif /* ADC_H */
