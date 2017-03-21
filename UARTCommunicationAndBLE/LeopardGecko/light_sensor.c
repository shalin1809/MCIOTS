/******************************************************************************
* File: light_sensor.c
*
* Created on: 22-Feb-2017
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
* 1. void I2C_TSL2561_Init();
* 2. void GPIO_ODD_IRQHandler();
******************************************************************************/

/*****************************************************
            * Include Statements *
 *****************************************************/
#include "light_sensor.h"




/************************************************************************
* Function to initialize the TSL2561 sensor
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: data
*
**************************************************************************/
void I2C_TSL2561_Init()
{
    CMU_ClockEnable(cmuClock_GPIO, true);                                               //Enable clocks to GPIO
    GPIO_PinModeSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN, gpioModePushPull, 1);        //power up the sensor
    GPIO_PinModeSet(TSL2561_INT_PORT, TSL2561_INT_PIN, gpioModeInput, 0);               //Initialize the interrupt pin
    GPIO_PinModeSet(I2C1_SCL_PORT, I2C1_SCL_PIN, gpioModeWiredAndPullUp, 1);            //Enable the SCL pin
    GPIO_PinModeSet(I2C1_SDA_PORT, I2C1_SDA_PIN, gpioModeWiredAndPullUp, 1);            //Enable the SDA pin
    I2C_Setup();                                                                        //Setup the I2C peripheral
    GPIO_IntConfig(TSL2561_INT_PORT,TSL2561_INT_PIN,false, true, true);                 //Setup interrupts for PD1
    NVIC_EnableIRQ(GPIO_ODD_IRQn);                                                      //Enable interrupts for GPIO ODD pins
}






/************************************************************************
* Turn off power supply to the TSL2561 powered via PD0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
void GPIO_ODD_IRQHandler()
{
    __disable_irq();
    uint8_t data;                   //Variable to store the read data
    data = TSL2561_Read();          //Read the higher byte
    GPIO_IntClear(GPIO->IF);        //Clear the GPIO interrupt
    if(data){                       //If brightness detected
        ledOFF(LIGHT_LED);          //Turn off LED
    }
    else{
        ledON(LIGHT_LED);           //Else turn on LED
    }
    __enable_irq();
}

