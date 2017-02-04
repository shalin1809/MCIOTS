/******************************************************************************
* File: cmu_setup.c
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
#include "cmu_setup.h"


/************************************************************************
* Setup and initialize the Clock Management Unit
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void CMU_Setup(void)
{
    if(MinimumEnergyMode == EM3)
    {
        CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);    //Enable ULFRCO clock for EM3
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); //Select ULFRCO as the clock for LFA clock tree to the letimer
    }
    else
    {
        CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      //Enable LXF0 clock oscillator
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   //Select LFXO as the clock for LFA clock tree to the letimer
    }
    CMU_ClockEnable(cmuClock_CORELE, true);                 //Enable clock to Low energy peripheral clock tree
    CMU_ClockEnable(cmuClock_LETIMER0, true);               //Enable clock to letimer0
}
