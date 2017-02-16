/******************************************************************************
* File: light_sensor.h
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
* 1. void Light_Sense_Init(void);
******************************************************************************/

#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_gpio.h"



/*****************************************************
 			* Define Statements *
 *****************************************************/
#define LIGHTSENSE_EXCITE_PORT    gpioPortD
#define LIGHTSENSE_EXCITE_PIN     6
#define LIGHTSENSE_PORT    gpioPortC
#define LIGHTSENSE_PIN     6



/************************************************************************
* Initialize and enable ambient Light Sensor
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
__STATIC_INLINE void Light_Sense_Init(void)
{
    GPIO_DriveModeSet(LIGHTSENSE_EXCITE_PORT, gpioDriveModeLowest);         //Set drive mode for excitation pin. Current less than 0.5mA required
    GPIO_DriveModeSet(LIGHTSENSE_PORT, gpioDriveModeLowest);                //Set drive mode for sensing pin. Current less than 0.5mA required

    GPIO_PinModeSet(LIGHTSENSE_EXCITE_PORT, LIGHTSENSE_EXCITE_PIN, gpioModePushPull, 0);    //Set gpio mode as pushpull
    GPIO_PinModeSet(LIGHTSENSE_PORT, LIGHTSENSE_PIN, gpioModeDisabled, 0);                  //gpio mode not required
    GPIO_PinOutSet(LIGHTSENSE_EXCITE_PORT,LIGHTSENSE_EXCITE_PIN);                           //Enable light sensor excitation
}





#endif /* LIGHT_SENSOR_H */
