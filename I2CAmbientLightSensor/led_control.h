/******************************************************************************
* File: led_control.h
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
* 1. void ledInit(void)
* 2. void ledON(unsigned int led);
* 3. void ledOFF(unsigned int led);
* 4. void ledToggle(unsigned int led);
******************************************************************************/

#ifndef LED_CONTROL_H
#define LED_CONTROL_H


/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_gpio.h"
#include "em_cmu.h"



/*****************************************************
            * Define Statements *
 *****************************************************/
#define LED0Port gpioPortE
#define LED0Pin 2
#define LED1Port gpioPortE
#define LED1Pin 3
#define LEDPort gpioPortE
#define LED_PIN_OFFSET 2


/************************************************************************
* Description: Initialize lEDs
*
* Input variables: LED number
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
__STATIC_INLINE void ledInit(void)
{
    CMU_ClockEnable(cmuClock_GPIO, true);                           //Enable clock to gpio
    GPIO_PinModeSet(LED0Port, LED0Pin, gpioModePushPull, 0);        //Initialize LED0
    GPIO_PinModeSet(LED1Port, LED1Pin, gpioModePushPull, 0);        //Initialize LED1
}


/************************************************************************
* Description: Turn On LED
*
* Input variables: LED number
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
__STATIC_INLINE void ledON(unsigned int led)
{
    GPIO_PinOutSet(LEDPort,(led+LED_PIN_OFFSET));                   //Set gpio pin high
}


/************************************************************************
* Description: Turn Off LED
*
* Input variables: LED number
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
__STATIC_INLINE void ledOFF(unsigned int led)
{
    GPIO_PinOutClear(LEDPort,(led+LED_PIN_OFFSET));                 //Set gpio pin low
}


/************************************************************************
* Description: Toggle LED
*
* Input variables: LED number
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
__STATIC_INLINE void ledToggle(unsigned int led)
{
    GPIO_PinOutToggle(LEDPort,(led+LED_PIN_OFFSET));                //Toggle gpio pin ouput
}

#endif /* LED_CONTROL_H */
