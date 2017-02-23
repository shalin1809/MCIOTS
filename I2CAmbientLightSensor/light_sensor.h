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
#include "i2c.h"
#include "em_cmu.h"
#include "stdint.h"
#include "led_control.h"

/*****************************************************
 			* Define Statements *
 *****************************************************/
#define LIGHTSENSE_EXCITE_PORT    gpioPortD
#define LIGHTSENSE_EXCITE_PIN     6
#define LIGHTSENSE_PORT    gpioPortC
#define LIGHTSENSE_PIN     6

#define ENABLE_PASSIVE_LIGHT_SENSOR false


#define TSL2561_THRESHOLD_LOW       0x000f
#define TSL2561_THRESHOLD_HIGH      0x0800

#define TSL2561_POWER_PORT          gpioPortD   //Port D pin 0
#define TSL2561_POWER_PIN           0           //Exp header 4
#define TSL2561_INT_PORT            gpioPortD   //port D pin 1
#define TSL2561_INT_PIN             1           //Exp header 6

#define TSL2561_ADDR                0x39        //Default sensor address
#define TSL2561_CONTROL_ADDR        0x00        //Control register address
#define TSL2561_TIMING_ADDR         0x01        //Timing register address
#define TSL2561_THRESHLOW_LOW_ADDR  0x02        //Low threshold lower byte address
#define TSL2561_THRESHLOW_HIGH_ADDR 0x03        //Low threshold higher byte address
#define TSL2561_THRESHIGH_LOW_ADDR  0x04        //High threshold lower byte
#define TSL2561_THRESHIGH_HIGH_ADDR 0x05        //High threshold higher byte
#define TSL2561_INTERRUPT_ADDR      0x06        //Interrupt register address
#define TSL2561_ID_ADDR             0x0A        //Device ID register address
#define TSL2561_DATA_0_LOW_ADDR     0x0C        //Data 0 lower byte address
#define TSL2561_DATA_0_HIGH_ADDR    0x0D        //Data 0 higher byte address
#define TSL2561_DATA_1_LOW_ADDR     0x0E        //Data 1 lower byte address
#define TSL2561_DATA_1_HIGH_ADDR    0x0F        //Data 1 higher byte address


#define TSL2561_CMD_CMD     0x80
#define TSL2561_CMD_CLEAR   0x40
#define TSL2561_CMD_WORD    0x20
#define TSL2561_CMD_BLOCK   0x10
#define TSL2561_CMD_ADDR    0x0F

#define TSL2561_CTRL_POWER_UP   0x03            //Power up the sensor
#define TSL2561_CTRL_POWER_DOWN 0x00            //Power down the sensor

#define TSL2561_TIMING_GAIN     0x00            //Select gain mode as low gain
#define TSL2561_TIMING_INTEG    0x01            //Integration time 101ms

#define TSL2561_INT_INTR        0x10            //Level interrupt mode
#define TSL2561_INT_PERSIST     0x04            //Interrupt persistence for 4 cycles

#define THRESHOLD_LOW_LOW       (TSL2561_THRESHOLD_LOW&0xFF)
#define THRESHOLD_LOW_HIGH      ((TSL2561_THRESHOLD_LOW&0xFF00)>>8)
#define THRESHOLD_HIGH_LOW      (TSL2561_THRESHOLD_HIGH&0xFF)
#define THRESHOLD_HIGH_HIGH     ((TSL2561_THRESHOLD_HIGH&0xFF00)>>8)

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

__STATIC_INLINE void TSL2561_PowerOn()
{
    GPIO_PinModeSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(I2C1_SCL_PORT, I2C1_SCL_PIN, gpioModeWiredAndPullUp, 1);
    GPIO_PinModeSet(I2C1_SDA_PORT, I2C1_SDA_PIN, gpioModeWiredAndPullUp, 1);
    GPIO_PinOutSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN);
}

__STATIC_INLINE void TSL2561_PowerOff()
{
    NVIC_DisableIRQ(GPIO_ODD_IRQn);
    GPIO_IntClear(GPIO->IF);
    GPIO_PinOutClear(TSL2561_POWER_PORT, TSL2561_POWER_PIN);
    GPIO_PinModeSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(I2C1_SCL_PORT, I2C1_SCL_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(I2C1_SDA_PORT, I2C1_SDA_PIN, gpioModeDisabled, 0);
}



__STATIC_INLINE void I2C1_Write(uint8_t address, uint8_t command, uint8_t data)
{
    I2C_IntClear(I2C1, I2C1->IF);
    I2C1->TXDATA = address;
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    I2C1->TXDATA = command;
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    I2C1->TXDATA = data;
    I2C1->CMD = I2C_CMD_NACK;
    I2C1->CMD = I2C_CMD_STOP;
    while(I2C1->STATUS & I2C_STATUS_PSTOP);
}

__STATIC_INLINE uint8_t I2C1_Read(uint8_t address, uint8_t command)
{
    uint8_t data = 0;
    I2C_IntClear(I2C1, I2C1->IF);
    I2C1->TXDATA = address;
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    I2C1->TXDATA = command;
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    I2C1->TXDATA = address|I2C_READ;
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    while((I2C1->STATUS & I2C_STATUS_RXDATAV) == 0);
    data = I2C1->RXDATA;
    I2C1->CMD = I2C_CMD_NACK;
    I2C1->CMD = I2C_CMD_STOP;
    while(I2C1->STATUS & I2C_STATUS_PSTOP);
    return data;
}


__STATIC_INLINE void TSL2561_PowerUp(void)
{

    I2C_IntClear(I2C1, I2C1->IF);
    I2C1->TXDATA = TSL2561_ADDR<<1 | I2C_WRITE;
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    I2C1->TXDATA = TSL2561_CONTROL_ADDR | TSL2561_CMD_CMD;
    while((I2C1->IF & I2C_IF_ACK) == 0);
        I2C1->IFC |= I2C_IFC_ACK;
    I2C1->TXDATA = TSL2561_CTRL_POWER_UP;
    I2C1->CMD = I2C_CMD_NACK;
    I2C1->CMD = I2C_CMD_STOP;
    while(I2C1->STATUS & I2C_STATUS_PSTOP);
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                    (TSL2561_CMD_CMD | TSL2561_TIMING_ADDR | TSL2561_CMD_CLEAR),
                    TSL2561_TIMING_GAIN| TSL2561_TIMING_INTEG);
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_THRESHLOW_LOW_ADDR | TSL2561_CMD_CLEAR),
                THRESHOLD_LOW_LOW);
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_THRESHLOW_HIGH_ADDR | TSL2561_CMD_CLEAR),
                THRESHOLD_LOW_HIGH);
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
               (TSL2561_CMD_CMD | TSL2561_THRESHIGH_LOW_ADDR | TSL2561_CMD_CLEAR),
               THRESHOLD_HIGH_LOW);
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
               (TSL2561_CMD_CMD | TSL2561_THRESHIGH_HIGH_ADDR | TSL2561_CMD_CLEAR),
               THRESHOLD_HIGH_HIGH);
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_INTERRUPT_ADDR | TSL2561_CMD_CLEAR),
                TSL2561_INT_INTR | TSL2561_INT_PERSIST);
    GPIO_IntClear(GPIO->IF);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
}



__STATIC_INLINE void I2C_TSL2561_Init()
{
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN, gpioModePushPull, 1);        //power up the sensor
    GPIO_PinModeSet(TSL2561_INT_PORT, TSL2561_INT_PIN, gpioModeInput, 0);               //Initialize the interrupt pin
    GPIO_PinModeSet(I2C1_SCL_PORT, I2C1_SCL_PIN, gpioModeWiredAndPullUp, 1);
    GPIO_PinModeSet(I2C1_SDA_PORT, I2C1_SDA_PIN, gpioModeWiredAndPullUp, 1);
    I2C_Setup();
    //TSL2561_PowerUp();
    GPIO_IntConfig(TSL2561_INT_PORT,TSL2561_INT_PIN,false, true, true);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);

}


__STATIC_INLINE uint8_t TSL2561_Read(){
    uint8_t data;
    I2C1_Read((TSL2561_ADDR<<1|I2C_WRITE),
            (TSL2561_CMD_CMD| TSL2561_CMD_CLEAR |TSL2561_DATA_0_LOW_ADDR));
    data = I2C1_Read((TSL2561_ADDR<<1|I2C_WRITE),
            (TSL2561_CMD_CMD| TSL2561_CMD_CLEAR |TSL2561_DATA_0_HIGH_ADDR));
    return data;
}





#endif /* LIGHT_SENSOR_H */
