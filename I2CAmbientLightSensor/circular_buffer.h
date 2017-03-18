/******************************************************************************
* File: circular_buffer.h
*
* Created on: 17-Mar-2017
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

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H




/*****************************************************
	 		* Include Statements *
 *****************************************************/
#include "em_device.h"
#include <stdint.h>
#include <stdbool.h>


/*****************************************************
 			* Define Statements *
 *****************************************************/

#define MAX_DATA_LENGTH     10
#define MAX_CMDS            10
#define MAX_SIZE            MAX_DATA_LENGTH*MAX_CMDS


/*****************************************************
            * Global Variables *
 *****************************************************/
uint8_t tx_buffer[MAX_CMDS*MAX_DATA_LENGTH];


typedef enum bufferStates{                        //Enumeration for the current state of buffer
    Buffer_Empty,
    Buffer_Full,
    Buffer_Ok
}bufferStates_t;


typedef enum CMDS_t{                        //Enumeration for the command packet
    LED_TOGGLE = 1,
    TEMPERATURE = 2,
}CMDS;



typedef struct CircBuf{
    uint8_t *buff;
    uint8_t *head;
    uint8_t *tail;
    uint8_t count;
}CircBuf_t;

CircBuf_t tbuff;
CircBuf_t *tx_buff;



/************************************************************************
* Description
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
* IP
**************************************************************************/
// This function checks if the buffer is empty
__STATIC_INLINE bufferStates_t is_Buffer_empty(CircBuf_t *circ_ptr){
    if(circ_ptr -> count == 0){
        return Buffer_Empty;
    }
    else{
        return Buffer_Ok;
    }
}


// This function checks if the buffer is full
__STATIC_INLINE bufferStates_t is_Buffer_full(CircBuf_t *circ_ptr){
    if(circ_ptr -> count == MAX_SIZE){
        return Buffer_Full;
    }
    else{
        return Buffer_Ok;
    }
}


// This function adds an item to the buffer array
__STATIC_INLINE bufferStates_t add_item(CircBuf_t *circ_ptr, uint8_t item){
    if(is_Buffer_full(circ_ptr) == Buffer_Full) {                 // if buffer is full, return a buffer full error
        return Buffer_Full;
    }
    circ_ptr->count += 1;                               // incrementing the number of elements
    if(circ_ptr->head == (circ_ptr -> buff + (MAX_SIZE -1))){
        circ_ptr -> head = circ_ptr -> buff;            // so that we wrap around after the last element
    }
    else
        circ_ptr -> head++;

    *(circ_ptr -> head) = item;              // assigning the item to the incremented head position
    return Buffer_Ok;
}


//This function removes the oldest element from the buffer
__STATIC_INLINE uint8_t remove_item(CircBuf_t *circ_ptr){
    if(is_Buffer_empty(circ_ptr) == 0){                         // if buffer is empty, do not add more elements.
        return Buffer_Empty;                                               // return 4 which is code for Buffer empty error
    }
    if(circ_ptr -> tail == (circ_ptr -> buff + (MAX_SIZE -1))){
        circ_ptr -> tail = circ_ptr -> buff;    // so that we can wrap around after the last element
    }
    else
        circ_ptr -> tail++;

    circ_ptr -> count--;                            // number of active elements is decreased
    return *(circ_ptr -> tail);                     // returning the value read
}



__STATIC_INLINE void circular_buffer_init(CircBuf_t *circ_ptr){
    circ_ptr-> buff = tx_buffer;
    circ_ptr-> head = circ_ptr-> buff;
    circ_ptr-> tail = circ_ptr-> buff;
}




#endif /* CIRCULAR_BUFFER_H */
