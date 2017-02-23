/*****************************************************************************
*   Name: Shalin Shah
*   Date: 1 Feb 2017
*   Assignment: Managing Energy Modes of the Leopard Gecko and getting familiar
*   with Simplicity Studio
*
*******************************************************************************
*	File: main.c
*	Description: The main executional block of the program
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
* 1. int main();
******************************************************************************/



/******************************************
 		* Include Statements *
 ******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "letimer.h"
#include "sleep.h"
#include "cmu_setup.h"
#include "led_control.h"
#include "light_sensor.h"
#include "acmp.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"


/*****************************************************
            * Define Statements *
 *****************************************************/
#define ON 1
#define OFF 0
#define Calibration ON                  //Macro to turn calibration ON or OFF
#define milliseconds 1000               //Macro for calibration time in milliseconds

#if !Calibration
    #define CMU_CAL_ULFRCO(x) __NOP()
    extern float clock_scale = 1;
#endif


/************************************************************************
* The start point of program execution
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
int main(void)
{
    CHIP_Init();						//Chip errata
    CMU_CAL_ULFRCO(milliseconds);       //Calibrate ULFRCO using the LXFO as reference
    CMU_Setup();						//Initialize all the clocks
    ledInit();                          //Initialize leds
    LETIMER_Setup();					//Initialize letimer
#if ENABLE_PASSIVE_LIGHT_SENSOR
    Light_Sense_Init();                 //Initialize Ambient Light Sensor
    ACMP0_Init();                       //Initialize ACMP
#endif
    Init_ADC0();                        //Initialize ADC0
    Init_DMA();
    I2C_TSL2561_Init();
    LETIMER_Enable(LETIMER0, true);     //Enable letimer0
    /* Infinite loop */
    while (1) {
        Sleep();						//Put to sleep after executing interrupts
    }
}
