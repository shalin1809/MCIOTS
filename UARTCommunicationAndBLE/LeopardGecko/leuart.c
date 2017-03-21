/******************************************************************************
* File: leuart.c
*
* Created on: 15-Mar-2017
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
* 1. void leuart_setup();
* 2. void LEUART0_IRQHandler(void);
******************************************************************************/




/*****************************************************
            * Include Statements *
 *****************************************************/
#include "leuart.h"



/************************************************************************
* Function to setup and initialize LEUART0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void leuart_setup(){

    GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);      //Initialize and set TX pin as output
    GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 1);         //Initialize and set RX pin as input

    LEUART_Init_TypeDef leuart_init =
    {
      .enable = false,                                  //Do not enable on initialization
      .refFreq = LEUART_NOREF,                          //Use current clock as reference for configuring the baud rate
      .baudrate = LEUART_BAUD,                          //Set the LEUART BAUD rate
      .databits = leuartDatabits8,                      //Use 8 data bits
      .parity = leuartNoParity,                         //Don't use any parity
      .stopbits = leuartStopbits1                       //Use 1 stop bit
    };

    LEUART_Init(LEUART0, &leuart_init);                 //Initialize the LEUART

    LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION; //Select location 0, PD4- TX, PD5- RX
    LEUART_IntDisable(LEUART0,LEUART_DIS_ALL_INT);      //Disable all interrupts
    LEUART_IntClear(LEUART0, LEUART_CLEAR_ALL_INT);     //Clear all interrupt flags
    LEUART_IntEnable(LEUART0, LEUART_IFC_TXC);          //Enable transmit complete interrupts
    NVIC_EnableIRQ(LEUART0_IRQn);                       //Enable LEUART in the NVIC table
    LEUART_Enable(LEUART0, true);                       //Enable LEUART
}



/************************************************************************
* LEUART0 IRQ Handler
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void LEUART0_IRQHandler(void){
    uint8_t byte;
    __disable_irq();                                            //Disable interrupts
    LEUART0->IFC = LEUART_CLEAR_ALL_INT;                        //Clear LEUART interrupts
    if(tx_buff-> count){                             //If there is anything in the buffer
        byte = remove_item(tx_buff);                            //Fetch one byte from the circular buffer
        LEUART0_Send_Byte(byte);                                //Send the byte
    }
    else{                                           //If transmission complete
        UnblockSleepMode(LEUART_EM);                            //Unblock sleep mode
        LEUART0->CMD = LEUART_CMD_RXDIS|LEUART_CMD_TXDIS;       //Disable RX and TX pins
    }
    __enable_irq();                                             //Re-enable interrupts
}

