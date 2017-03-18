/******************************************************************************
* File: dma.h
*
* Created on: 14-Feb-2017
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
* 1. void Init_DMA();
* 2. void ADC0_DMA_Init();
******************************************************************************/

#ifndef DMA_H
#define DMA_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_dma.h"
#include "em_device.h"
#include "dmactrl.c"
#include "adc.h"
#include "circular_buffer.h"
#include "leuart.h"

/*****************************************************
 			* Define Statements *
 *****************************************************/
#define ADC0_DMA_Arbitration dmaArbitrate1              //0 R_BIT value for ADC0 DMA
#define ADC0_DMA_Channel 0                              //DMA Channel used for ADC0
#define BYTES_2 dmaDataSize2                            //16-bit data size
#define INC_BYTES_2 dmaDataInc2                         //16-bit data increment


/*****************************************************
            * Global Variables *
 *****************************************************/
uint16_t ADC0_DMA_buffer[NUMBER_OF_ADC_SAMPLES];        //Buffer to store the ADC0 DMA results
DMA_CB_TypeDef ADC0_cb;                                 //Callback structure for DMA


/************************************************************************
* Initialize DMA for ADC
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
void Init_DMA();


/************************************************************************
* Initialize DMA for ADC
*
* Input variables: None
*
* Global variables: ADC0_cb
*
* Returned variables: None
*
**************************************************************************/
void ADC0_DMA_Init();


/************************************************************************
* Callback routine for DMA complete
*
* Input variables: channel, primary, *user
*
* Global variables: adcSum, count
*
* Returned variables: None
*
**************************************************************************/
void ADC0_DMA_Done(unsigned int channel, bool primary, void *user);


#endif /* DMA_H */
