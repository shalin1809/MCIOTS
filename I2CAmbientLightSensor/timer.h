/******************************************************************************
* File: timer.h
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
* emlib library of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
*
* The functions that use this library are:
* 1. void TIMER0_SETUP(void);
* 2. void TIMER1_SETUP(void);
******************************************************************************/

#ifndef TIMER_H
#define TIMER_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_timer.h"
#include <stdbool.h>
#include <stdint.h>



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
void TIMER0_SETUP(void);

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
void TIMER1_SETUP(void);

#endif /* TIMER_H */
