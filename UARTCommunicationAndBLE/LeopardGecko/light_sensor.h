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
* 2. void TSL2561_PowerOn();
* 3. void TSL2561_PowerOff();
* 4. void I2C_TSL2561_Init();
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
#include "leuart.h"

/*****************************************************
 			* Define Statements *
 *****************************************************/
#define LIGHTSENSE_EXCITE_PORT      gpioPortD
#define LIGHTSENSE_EXCITE_PIN       6
#define LIGHTSENSE_PORT             gpioPortC
#define LIGHTSENSE_PIN              6
#define LIGHT_LED                   0

#define ENABLE_PASSIVE_LIGHT_SENSOR true


#define TSL2561_THRESHOLD_LOW       0x000f      //External ambient light sensor low threshold
#define TSL2561_THRESHOLD_HIGH      0x0800      //External ambient light sensor high threshold

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


#define TSL2561_CMD_CMD     0x80                //Command byte command mask
#define TSL2561_CMD_CLEAR   0x40                //Command byte clear mask
#define TSL2561_CMD_WORD    0x20                //Command byte word mask
#define TSL2561_CMD_BLOCK   0x10                //Command byte block mode mask
#define TSL2561_CMD_ADDR    0x0F                //Command byte device register address

#define TSL2561_CTRL_POWER_UP   0x03            //Power up the sensor
#define TSL2561_CTRL_POWER_DOWN 0x00            //Power down the sensor

#define TSL2561_TIMING_GAIN     0x00            //Select gain mode as low gain
#define TSL2561_TIMING_INTEG    0x01            //Integration time 101ms

#define TSL2561_INT_INTR        0x10            //Level interrupt mode
#define TSL2561_INT_PERSIST     0x04            //Interrupt persistence for 4 cycles

#define THRESHOLD_LOW_LOW       (TSL2561_THRESHOLD_LOW&0xFF)            //Lower byte of the low threshold
#define THRESHOLD_LOW_HIGH      ((TSL2561_THRESHOLD_LOW&0xFF00)>>8)     //Higher byte of the low threshold
#define THRESHOLD_HIGH_LOW      (TSL2561_THRESHOLD_HIGH&0xFF)           //Lower byte of the high threshold
#define THRESHOLD_HIGH_HIGH     ((TSL2561_THRESHOLD_HIGH&0xFF00)>>8)    //Higher byte of the high threshold

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
void I2C_TSL2561_Init();


/************************************************************************
* Turn on power supply to the TSL2561 powered via PD0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
__STATIC_INLINE void TSL2561_PowerOn()
{
    GPIO_PinModeSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN, gpioModePushPull, 1);    //Turn on power supply
    GPIO_PinModeSet(I2C1_SCL_PORT, I2C1_SCL_PIN, gpioModeWiredAndPullUp, 1);        //Turn on SCL for I2C
    GPIO_PinModeSet(I2C1_SDA_PORT, I2C1_SDA_PIN, gpioModeWiredAndPullUp, 1);        //Turn on SDA for I2C
    GPIO_PinModeSet(TSL2561_INT_PORT, TSL2561_INT_PIN, gpioModeInput, 1);           //Initialize the interrupt pin
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
__STATIC_INLINE void TSL2561_PowerOff()
{
    NVIC_DisableIRQ(GPIO_ODD_IRQn);                                                 //Disable interrupts
    GPIO_IntClear(GPIO->IF);                                                        //Clear interrupts
    GPIO_PinModeSet(TSL2561_POWER_PORT, TSL2561_POWER_PIN, gpioModeDisabled, 0);    //Disable the power pin
    GPIO_PinOutClear(TSL2561_POWER_PORT, TSL2561_POWER_PIN);                        //Clear the power pin to ground value
    GPIO_PinModeSet(TSL2561_INT_PORT, TSL2561_INT_PIN, gpioModeDisabled, 0);        //Disable the interrupt pin
    GPIO_PinModeSet(I2C1_SCL_PORT, I2C1_SCL_PIN, gpioModeDisabled, 0);              //Disable the SCL pin
    GPIO_PinModeSet(I2C1_SDA_PORT, I2C1_SDA_PIN, gpioModeDisabled, 0);              //Disable the I2C pin
}





/************************************************************************
* Single Write to the registers of TSL2561
*
* Input variables: address, command, data
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
__STATIC_INLINE void I2C1_Write(uint8_t address, uint8_t command, uint8_t data)
{
    I2C_IntClear(I2C1, I2C1->IF);                   //Clear all interrupts
    I2C1->TXDATA = address;                         //Set the slave address to the transmit buffer
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    while((I2C1->IF & I2C_IF_ACK) == 0);            //Wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    I2C1->TXDATA = command;                         //Send the command for TSL2561
    while((I2C1->IF & I2C_IF_ACK) == 0);            //Wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    I2C1->TXDATA = data;                            //Send data to the device resiter
    I2C1->CMD = I2C_CMD_NACK;                       //Send NACK to end transmission
    I2C1->CMD = I2C_CMD_STOP;                       //Send stop bit to complete
    while(I2C1->STATUS & I2C_STATUS_PSTOP);         //Wait for successful stop transmission
}




/************************************************************************
* Single read from the registers of TSL2561
*
* Input variables: address, command
*
* Global variables: None
*
* Returned variables: data
*
**************************************************************************/
__STATIC_INLINE uint8_t I2C1_Read(uint8_t address, uint8_t command)
{
    uint8_t data = 0;                               //Temporary variable to store the received data
    I2C_IntClear(I2C1, I2C1->IF);                   //Clear all interrupts
    I2C1->TXDATA = address;                         //Set the slave address to the transmit buffer
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    while((I2C1->IF & I2C_IF_ACK) == 0);            //Wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    I2C1->TXDATA = command;                         //Send the command for TSL2561
    while((I2C1->IF & I2C_IF_ACK) == 0);            //Wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    I2C1->CMD = I2C_CMD_START;                      //Send re-start bit
    I2C1->TXDATA = address|I2C_READ;                //Send slave address with the read bit high
    while((I2C1->IF & I2C_IF_ACK) == 0);            //Wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    while((I2C1->STATUS & I2C_STATUS_RXDATAV) == 0);//Wait for receive data valid
    data = I2C1->RXDATA;                            //Read the data from the received buffer
    I2C1->CMD = I2C_CMD_NACK;                       //Send NACK to end transmission
    I2C1->CMD = I2C_CMD_STOP;                       //Send stop bit to complete
    while(I2C1->STATUS & I2C_STATUS_PSTOP);         //Wait for successful stop transmission
    return data;                                    //return the read data
}



/************************************************************************
* Routine to setup and power up the TSL2561 sensor
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
**************************************************************************/
__STATIC_INLINE void TSL2561_PowerUp(void)
{
    I2C_IntClear(I2C1, I2C1->IF);                   //Clear all interrupts
    I2C1->TXDATA = TSL2561_ADDR<<1 | I2C_WRITE;     //Set the slave address to the transmit buffer
    I2C1->CMD = I2C_CMD_START;                      //Send start bit
    while((I2C1->IF & I2C_IF_ACK) == 0);            //wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    I2C1->TXDATA = TSL2561_CONTROL_ADDR | TSL2561_CMD_CMD;  //Send the command for TSL2561
    while((I2C1->IF & I2C_IF_ACK) == 0);            //Wait for acknowledge
        I2C1->IFC |= I2C_IFC_ACK;                   //Clear acknowledge interrupt
    I2C1->TXDATA = TSL2561_CTRL_POWER_UP;           //Send power up command to the control register
    I2C1->CMD = I2C_CMD_NACK;                       //Send NACK to end transmission
    I2C1->CMD = I2C_CMD_STOP;                       //Send stop bit to complete
    while(I2C1->STATUS & I2C_STATUS_PSTOP);         //Wait for successful stop transmission

    //Set the gain to low and timing integration to 101ms
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_TIMING_ADDR | TSL2561_CMD_CLEAR),
                TSL2561_TIMING_GAIN| TSL2561_TIMING_INTEG);

    //Set lower byte of the low threshold register
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_THRESHLOW_LOW_ADDR | TSL2561_CMD_CLEAR),
                THRESHOLD_LOW_LOW);
    //Set higher byte of the low threshold register
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_THRESHLOW_HIGH_ADDR | TSL2561_CMD_CLEAR),
                THRESHOLD_LOW_HIGH);
    //Set lower byte of the high threshold register
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
               (TSL2561_CMD_CMD | TSL2561_THRESHIGH_LOW_ADDR | TSL2561_CMD_CLEAR),
               THRESHOLD_HIGH_LOW);
    //Set higher byte of the high threshold register
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
               (TSL2561_CMD_CMD | TSL2561_THRESHIGH_HIGH_ADDR | TSL2561_CMD_CLEAR),
               THRESHOLD_HIGH_HIGH);

    //Set the interrupt and persistance
    I2C1_Write(((TSL2561_ADDR << 1 )| I2C_WRITE),
                (TSL2561_CMD_CMD | TSL2561_INTERRUPT_ADDR | TSL2561_CMD_CLEAR),
                TSL2561_INT_INTR | TSL2561_INT_PERSIST);
    GPIO_IntClear(GPIO->IF);                        //Clear GPIO interrupts
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);            //Clear all pending interrupts
    NVIC_EnableIRQ(GPIO_ODD_IRQn);                  //Enable GPIO interrupts
}




/************************************************************************
* Function to read the data registers of the TSL2561 sensor
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: data
*
**************************************************************************/
__STATIC_INLINE uint8_t TSL2561_Read(){
    uint8_t data;
    I2C1_Read((TSL2561_ADDR<<1|I2C_WRITE),
            (TSL2561_CMD_CMD| TSL2561_CMD_CLEAR |TSL2561_DATA_0_LOW_ADDR));
    data = I2C1_Read((TSL2561_ADDR<<1|I2C_WRITE),
            (TSL2561_CMD_CMD| TSL2561_CMD_CLEAR |TSL2561_DATA_0_HIGH_ADDR));
    return data;
}


#endif /* LIGHT_SENSOR_H */
