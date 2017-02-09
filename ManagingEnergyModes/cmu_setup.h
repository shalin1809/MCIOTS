/******************************************************************************
* File: cmu_setup.h
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

#ifndef CMU_SETUP_H
#define CMU_SETUP_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_cmu.h"
#include "letimer.h"



/*****************************************************
 			* Define Statements *
 *****************************************************/




/************************************************************************
* Setup and initialize the Clock Management Unit
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void CMU_Setup(void);



#endif /* SRC_CMU_SETUP_H_ */