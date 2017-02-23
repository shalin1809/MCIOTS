/******************************************************************************
* File: sleep.c
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
******************************************************************************/

/*****************************************************
            * Include Statements *
 *****************************************************/
#include "sleep.h"

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
void UnblockSleepMode(EnergyMode eMode)
{
    __disable_irq();                            //Disable interrupts to make the operation atomic
    if(sleep_block_counter[eMode] > 0)
    {
        sleep_block_counter[eMode]--;           //Reduce the block counter of the input energy mode if blocked
    }
    __enable_irq();                             //Re-enable interrupts
}

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
void BlockSleepMode(EnergyMode minimumMode)
{
    __disable_irq();                            //Disable interrupts to make the operation atomic
    sleep_block_counter[minimumMode]++;         //Increase the block counter of the input energy mode
    __enable_irq();                             //Re-enable interrupts
}

