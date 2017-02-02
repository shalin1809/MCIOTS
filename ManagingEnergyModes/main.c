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
* BSP and emlib libraries of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
*
* 1. The functions that use this library are:
* 2. void UnblockSleepMode(EnergyMode eMode);
* 3. void BlockSleepMode(EnergyMode eMode);
* 4. void Sleep(minimumMode);
* 5. void CMU_Setup(void);
* 6. void LETIMER_Setup(void);
* 7. void LETIMER0_IRQHandler(void);
* 8. int main();
******************************************************************************/



/******************************************
 		* Include Statements *
 ******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "bsp.h"
#include "em_cmu.h"
#include "em_letimer.h"
#include "em_int.h"
#include "em_emu.h"



/******************************************
 		* Enumerations *
 ******************************************/
typedef enum EnergyMode_t{  		//Enumerating Energy Modes
	EM0 = 0,
	EM1 = 1,
	EM2 = 2,
	EM3 = 3,
	EM4 = 4
}EnergyMode;



/******************************************
 		* Define Statements *
 ******************************************/
#define MinimumEnergyMode EM3		//Minimum energy mode the system can enter, change this to change the minimum energy mode
#define NumberOfEnergyModes 4		//Number of Energy modes available for the Leopard Gecko
#define LEDPeriod 1.75				//Period between LED0 being turned on in seconds
#define LEDOnTime 0.03				//Led on time duration in seconds, should be less than period


/******************************************
 		* Global Variables *
 ******************************************/
char sleep_block_counter[NumberOfEnergyModes];	//Global of array sleep block counter for each energy mode
int CurrentClockFreq;							//Variable to store current tree clock frequency



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
	INT_Disable();								//Disable interrupts to make the operation atomic
	if(sleep_block_counter[eMode] > 0)
	{
		sleep_block_counter[eMode]--;			//Reduce the block counter of the input energy mode if blocked
	}
	INT_Enable();								//Re-enable interrupts
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
	INT_Disable();								//Disable interrupts to make the operation atomic
	sleep_block_counter[minimumMode]++;			//Increase the block counter of the input energy mode
	INT_Enable();								//Re-enable interrupts
}

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
void Sleep(void)
{
	if(sleep_block_counter[0]>0)			//If EM0 blocked, do nothing
		return;
	else if(sleep_block_counter[1]>0)		//If EM1 blocked, enter EM1
		EMU_EnterEM1();
	else if(sleep_block_counter[2]>0)		//If EM2 blocked, enter EM2
		EMU_EnterEM2(true);
	else if(sleep_block_counter[3]>0)		//If EM3 blocked, enter EM3
		EMU_EnterEM3(true);
	else									//If there are no blocks, do nothing, avoid entering EM4
		return;
}

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
		CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true); 	//Enable ULFRCO clock for EM3
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);	//Select ULFRCO as the clock for LFA clock tree to the letimer
	}
	else
	{
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true); 		//Enable LXF0 clock oscillator
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);	//Select LFXO as the clock for LFA clock tree to the letimer
	}
	CMU_ClockEnable(cmuClock_CORELE, true);					//Enable clock to Low energy peripheral clock tree
	CMU_ClockEnable(cmuClock_LETIMER0, true);				//Enable clock to letimer0
}

/************************************************************************
* Setup and initialize the LETIMER
*
* Input variables: None
*
* Global variables: CurrentClockFreq
*
* Returned variables: None
**************************************************************************/
void LETIMER_Setup(void)
{
	int Period, OnTime;							//Variables for Period and Ontime
	const LETIMER_Init_TypeDef letimerInit =
    {
    	.enable         = true,             	//Start counting when init completed
		.debugRun       = true,                 //Counter shall not keep running during debug halt
		.rtcComp0Enable = false,                //Don't start counting on RTC COMP0 match
		.rtcComp1Enable = false,                //Don't start counting on RTC COMP1 match
		.comp0Top       = true,                 //Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP
		.bufTop         = false,                //Don't load COMP1 into COMP0 when REP0 reaches 0
		.out0Pol        = 0,                    //Idle value for output 0.
		.out1Pol        = 0,                   	//Idle value for output 1.
		.ufoa0          = letimerUFOANone,     	//PWM output on output 0
		.ufoa1          = letimerUFOANone,     	//Pulse output on output 1
		.repMode        = letimerRepeatFree    	//Count until stopped
    };

	CurrentClockFreq = CMU_ClockFreqGet(cmuClock_LFA);
	Period = (CurrentClockFreq * LEDPeriod);
	LETIMER_CompareSet(LETIMER0, 0, Period);
	OnTime = (CurrentClockFreq * LEDOnTime);
	LETIMER_CompareSet(LETIMER0, 1, OnTime);

	LETIMER_Init(LETIMER0, &letimerInit);
	while(LETIMER0->SYNCBUSY);

	LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1);
	LETIMER_IntEnable(LETIMER0 , LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);
	NVIC_EnableIRQ(LETIMER0_IRQn);
	BlockSleepMode(MinimumEnergyMode);
}

/************************************************************************
* Interrupt handler routine for LETIMER0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void LETIMER0_IRQHandler(void)
{
	int intFlags;							//Variable to store the interrupting flags
	INT_Disable();							//Diable interrupts to avoid nesting
	intFlags = LETIMER_IntGet(LETIMER0);	//Get the interrupting flags
	LETIMER_IntClear(LETIMER0, intFlags);	//Clear the interrupting flags
	if(intFlags & LETIMER_IF_COMP0)			//If COMP0 interrupts, turn off the LED
		BSP_LedClear(0);
	else
		BSP_LedSet(0);						//COMP1 interrupts, turn on LED
	INT_Enable();							//Re-enable interrupts
}

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
  BSP_LedsInit();					//Initialize Leds
  CMU_Setup();						//Initialize all the clocks
  LETIMER_Setup();					//Initialize letimer
  LETIMER_Enable(LETIMER0, true);	//Enable letimer0

  /* Infinite loop */
  while (1) {
	 Sleep();						//Put to sleep after executing interrupts
  }
}
