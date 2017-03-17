/******************************************************************************
* File: leuart.h
*
* Created on: 09-Mar-2017
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
* 1.
******************************************************************************/

#ifndef LEUART_H
#define LEUART_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_leuart.h"
#include "em_gpio.h"
#include "circular_buffer.h"
#include <stdbool.h>
#include <stdint.h>


/*****************************************************
 			* Define Statements *
 *****************************************************/
#define LEUART_LOCATION     0
#define LEUART_TXPORT       gpioPortD           //Port D pin 4
#define LEUART_TXPIN        4                   //Exp header 12
#define LEUART_RXPORT       gpioPortD           //Port D pin 5
#define LEUART_RXPIN        5                   //Exp header 14
#define LEUART_BAUD         9600
#define LEUART_NOREF        0
#define LEUART_CLEAR_ALL_INT    0x7F9           //Clear all interrupt flags
#define LEUART_DIS_ALL_INT  0x7FF               //Disable all interrupt flags




/************************************************************************
* Description
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
* IP
**************************************************************************/
void leuart_setup();



__STATIC_INLINE void LEUART0_Send_Byte(uint8_t byte){
    LEUART0->TXDATA = byte;                             //Write the byte to the transmit data buffer
}



#endif /* LEUART_H */
