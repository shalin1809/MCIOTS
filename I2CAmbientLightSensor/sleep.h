/******************************************************************************
* File: sleep.h
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
* 2. void UnblockSleepMode(EnergyMode eMode);
* 3. void BlockSleepMode(EnergyMode eMode);
* 4. void Sleep(minimumMode);
******************************************************************************/

#ifndef SLEEP_H
#define SLEEP_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_emu.h"



/******************************************
        * Enumerations *
 ******************************************/
typedef enum EnergyMode_t{          //Enumerating Energy Modes
    EM0 = 0,
    EM1 = 1,
    EM2 = 2,
    EM3 = 3,
    EM4 = 4
}EnergyMode;



/*****************************************************
 			* Define Statements *
 *****************************************************/
#define NumberOfEnergyModes 4       //Number of Energy modes available for the Leopard Gecko




/******************************************
        * Global Variables *
 ******************************************/
char sleep_block_counter[NumberOfEnergyModes];  //Global of array sleep block counter for each energy mode


/************************************************************************
* Function to unblock sleep modes
*
* Input variables: (enum EnergyMode)eMode for the energy mode to be unblocked
*
* Global variables: sleep_block_counter
*
* Returned variables: None
*
* Code source: ECEN 5023-001 Lecture3.pdf by Keith Graham
**************************************************************************/
void UnblockSleepMode(EnergyMode eMode);


/************************************************************************
* Function to block sleep modes
*
* Input variables: (enum)minimumMode for the energy mode to be blocked
*
* Global variables: sleep_block_counter
*
* Returned variables: None
*
* Code source: ECEN 5023-001 Lecture3.pdf by Keith Graham
**************************************************************************/
void BlockSleepMode(EnergyMode minimumMode);


/************************************************************************
* Function to put the SoC to the minimum permitted sleep mode
*
* Input variables: None
*
* Global variables: sleep_block_counter
*
* Returned variables: None
*
* Code source: ECEN 5023-001 Lecture3.pdf by Keith Graham
**************************************************************************/
__STATIC_INLINE void Sleep(void)
{
    if(*sleep_block_counter>0)              //If EM0 blocked, do nothing
        return;
    else if(*(sleep_block_counter+1)>0)     //If EM1 blocked, enter EM1
        EMU_EnterEM1();
    else if(*(sleep_block_counter+2)>0)     //If EM2 blocked, enter EM2
        EMU_EnterEM2(true);
    else if(*(sleep_block_counter+3)>0)     //If EM3 blocked, enter EM3
        EMU_EnterEM3(true);
    else                                    //If there are no blocks, do nothing, avoid entering EM4
        return;
}


#endif /* SLEEP_H */
