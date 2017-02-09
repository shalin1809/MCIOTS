/******************************************************************************
* File: acmp.h
*
* Created on: 08-Feb-2017
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
* 1. void ACMP0_Init(void);
******************************************************************************/

#ifndef ACMP_H
#define ACMP_H



/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_acmp.h"



/*****************************************************
 			* Define Statements *
******************************************************/
#define LOW_THRESHOLD 2                     //Lower threshold value for ACMP
#define HIGH_THRESHOLD 61                   //Higher threshold value for ACMP

/*****************************************************
            * Global Structure*
******************************************************/
static ACMP_Init_TypeDef acmpInit =         // Creating struct to set the TIMER0 parameters
{
    .fullBias                 = false,      //Disable full bias mode
    .halfBias                 = true,       //Use only halfBias for lowest current consumption
    .biasProg                 = 0x0,        //Set BIASPROG to 0
    .interruptOnFallingEdge   = false,      // No comparator interrupts
    .interruptOnRisingEdge    = false,
    .warmTime                 = acmpWarmTime256,        //For 10uS using 14Mhz clock, >140 required
    .hysteresisLevel          = acmpHysteresisLevel4,   // Some hysteresis to prevent unnecessary toggling.
    .inactiveValue            = false,      //Nor required
    .lowPowerReferenceEnabled = true,       //Lowest power consumption
    .vddLevel                 = LOW_THRESHOLD,         //Set to high threshold
    .enable                   = false       //Not enabled at Init, will be enabled when required
};




/************************************************************************
* Initialize ACMP0 and enable
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
void ACMP0_Init(void);


#endif /* ACMP_H */
