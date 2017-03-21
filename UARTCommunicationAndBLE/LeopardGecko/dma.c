/******************************************************************************
* File: dma.c
*
* Created on: 15-Feb-2017
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


/*****************************************************
            * Include Statements *
 *****************************************************/
#include "dma.h"


/************************************************************************
* Initialize DMA
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
void Init_DMA()
{
    /* initialize DMA descriptor block location */
    DMA_Init_TypeDef DMA_init =
    {
        .controlBlock = dmaControlBlock,                //Define the dma control block
        .hprot = 0                                      //No channel protection required
    };
    DMA_Init(&DMA_init);                                //Initialize the dma

    ADC0_DMA_Init();                                    //Initialize ADC for DMA

    NVIC_EnableIRQ(DMA_IRQn);                           //Enable interrupts for DMA
}



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
void ADC0_DMA_Init()
{
    ADC0_cb.cbFunc = ADC0_DMA_Done;             //Assign the callback function pointer
    ADC0_cb.userPtr = NULL;                     //No user pointer required
    ADC0_cb.primary = true;                     //Use primary descriptor

    /* Configure DMA descriptor */
    DMA_CfgDescr_TypeDef ADC0_DMA_cfg =
    {
        .arbRate = ADC0_DMA_Arbitration,                    //Set arbitration to 0
        .dstInc = INC_BYTES_2,                              //Increase destination address by 2 bytes
        .hprot = 0,                                         //No channel protection required
        .size = BYTES_2,                                    //Increase data size by 2 bytes
        .srcInc = dmaDataIncNone                            //Don not increment source address
    };
    DMA_CfgDescr(ADC0_DMA_Channel, true, &ADC0_DMA_cfg);    //Initialize the configure descriptor

    DMA_CfgChannel_TypeDef ADC0_DMA_channel =
    {
        .cb = &ADC0_cb,                                     //Assign the callback function pointer
        .enableInt = true,                                  //Enable interrupts
        .highPri = true,                                    //Set as high priority
        .select = DMAREQ_ADC0_SINGLE                        //Select ADC0 singlemode as input
    };

    DMA_CfgChannel(ADC0_DMA_Channel, &ADC0_DMA_channel);    //Initialize the channel configuration

    DMA->IFC = 1 << ADC0_DMA_Channel;                       //Clear all interrupt flags
    DMA->IEN = 1 << ADC0_DMA_Channel;                       //Enable interrupt for DMA complete
}

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
void ADC0_DMA_Done(unsigned int channel, bool primary, void *user)
{
    __disable_irq();
    ADC0->CMD = ADC_CMD_SINGLESTOP;                         //Stop ADC0
    DMA->CONFIG &= ~DMA_CONFIG_EN;                          //Disable DMA
    DMA->IFC = 1 << ADC0_DMA_Channel;                       //Clear DMA interrupt flag

    int count = NUMBER_OF_ADC_SAMPLES;                      //Initialize count to number of samples
    adcSum = 0;                                             //Initialize adcSum to 0
    while(count)
        adcSum += ADC0_DMA_buffer[count--];                 //Sum the ADC0 buffer values
    UnblockSleepMode(ADC_EM);                               //Unblock sleep mode
    union temperature_t{                                    //union to access individual bytes of the float temperature
        float temp;
        struct bytes_t{
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
            uint8_t byte4;
        }bytes;
    }temperature;
    temperature.temp = convertToCelcius();                  //Get the temperature in celsius using adcSum

    LEUART0->CMD = LEUART_CMD_TXEN;                         //Enable UART tx pin
    BlockSleepMode(LEUART_EM);                              //Block sleep mode to EM1
    add_item(tx_buff,temperature.bytes.byte1);              //Add the float value of temperature bytewize
    add_item(tx_buff,temperature.bytes.byte2);
    add_item(tx_buff,temperature.bytes.byte3);
    add_item(tx_buff,temperature.bytes.byte4);
    add_item(tx_buff,led0_state);                           //Add current state of led0 to the buffer
    LEUART0->IFS = LEUART_IFS_TXC;                          //Set transmit complete interrupt to trigger transmission of data

    if(TEMP_LOW_THRESHOLD > temperature.temp || temperature.temp > TEMP_HIGH_THRESHOLD) //If temperature exceeds boundary conditions
    {
        ledON(1);                                           //Turn on LED 1
    }
    else                                                    //Temperature is within bounds
        ledOFF(1);                                          //Turn off LED 1
    __enable_irq();
}

