/******************************************************************************
* File: i2c.h
*
* Created on: 21-Feb-2017
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

#ifndef I2C_H
#define I2C_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_i2c.h"
#include "em_gpio.h"


/*****************************************************
 			* Define Statements *
 *****************************************************/
#define I2C1_SCL_PORT   gpioPortC            //port C pin 5
#define I2C1_SCL_PIN    5                    //Exp header 9
#define I2C1_SDA_PORT   gpioPortC            //Port C pin 4
#define I2C1_SDA_PIN    4                    //Exp header 7
#define TSL_I2C         I2C1
#define I2C_READ        0x01
#define I2C_WRITE       0



/************************************************************************
* Description
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
__STATIC_INLINE void I2C_Setup()
{
    I2C_Init_TypeDef i2c_Init =
    {
        .enable = false,                    //Enable I2C peripheral when init completed
        .master = true,                     //Set to master
        .refFreq = 0,                       //Use currently configured clock as reference
        .freq = I2C_FREQ_STANDARD_MAX,      //Max frequency in standard mode
        .clhr = i2cClockHLRStandard         //Standard clock low high ratio
    };
    I2C1->ROUTE |= I2C_ROUTE_LOCATION_LOC0 | I2C_ROUTE_SCLPEN |I2C_ROUTE_SDAPEN;
    I2C_Init(I2C1, &i2c_Init);
    if(I2C1->STATE & I2C_STATE_BUSY)
        I2C1->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX | I2C_CMD_ABORT;  //Clear pending commands and transmit buffer. Write only register
    I2C_IntClear(I2C1, I2C1->IF);
    I2C_Enable(I2C1, true);


}





#endif /* I2C_H */
