/******************************************************************************
* File: circular_buffer.h
*
* Created on: 17-Mar-2017
* Author: Shalin Shah
* 
*******************************************************************************
* @section License
* <b>Copyright 2017 Shalin Shah
*******************************************************************************/


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
uint8_t tx_buffer[MAX_CMDS*MAX_DATA_LENGTH];        //Buffer to allocate memory for the circular buffer


typedef enum bufferStates{                          //Enumeration for the current state of buffer
    Buffer_Empty,
    Buffer_Full,
    Buffer_Ok
}bufferStates_t;


typedef enum CMDS_t{                                //Enumeration for the command packet
    LIGHT_LED_ON = 1,
    LIGHT_LED_OFF = 2,
    TEMPERATURE = 3
}CMDS;


typedef struct CircBuf{                             //Structure for circular buffer operations
    uint8_t *buff;
    uint8_t *head;
    uint8_t *tail;
    uint8_t count;
}CircBuf_t;

CircBuf_t tbuff;                                    //Transmit buffer structure
CircBuf_t *tx_buff;                                 //Pointer to the transmit buffer



/************************************************************************
* Function to check if the buffer is empty
*
* Input variables: CircBuf_t *circ_ptr
*
* Global variables: None
*
* Returned variables: bufferStates_t
**************************************************************************/
__STATIC_INLINE bufferStates_t is_Buffer_empty(CircBuf_t *circ_ptr){
    if(circ_ptr -> count == 0){
        return Buffer_Empty;
    }
    else{
        return Buffer_Ok;
    }
}


/************************************************************************
* Function to check if the buffer is full
*
* Input variables: CircBuf_t *circ_ptr
*
* Global variables: None
*
* Returned variables: bufferStates_t
**************************************************************************/
__STATIC_INLINE bufferStates_t is_Buffer_full(CircBuf_t *circ_ptr){
    if(circ_ptr -> count == MAX_SIZE){
        return Buffer_Full;
    }
    else{
        return Buffer_Ok;
    }
}



/************************************************************************
* Function to add an item to the buffer array
*
* Input variables: CircBuf_t *circ_ptr, uint8_t item
*
* Global variables: None
*
* Returned variables: bufferStates_t
**************************************************************************/
__STATIC_INLINE bufferStates_t add_item(CircBuf_t *circ_ptr, uint8_t item){
    if(is_Buffer_full(circ_ptr) == Buffer_Full) {               // if buffer is full, return a buffer full error
        return Buffer_Full;
    }
    circ_ptr->count += 1;                                       // incrementing the number of elements
    if(circ_ptr->head == (circ_ptr -> buff + (MAX_SIZE -1))){
        circ_ptr -> head = circ_ptr -> buff;                    // so that we wrap around after the last element
    }
    else
        circ_ptr -> head++;

    *(circ_ptr -> head) = item;                                 // assigning the item to the incremented head position
    return Buffer_Ok;
}



/************************************************************************
* Function to remove an item from the buffer array
*
* Input variables: CircBuf_t *circ_ptr
*
* Global variables: None
*
* Returned variables: uint8_t
**************************************************************************/
__STATIC_INLINE uint8_t remove_item(CircBuf_t *circ_ptr){
    if(is_Buffer_empty(circ_ptr) == 0){                         // if buffer is empty, do not add more elements.
        return Buffer_Empty;                                    // return Buffer empty error
    }
    if(circ_ptr -> tail == (circ_ptr -> buff + (MAX_SIZE -1))){
        circ_ptr -> tail = circ_ptr -> buff;                    // so that we can wrap around after the last element
    }
    else
        circ_ptr -> tail++;

    circ_ptr -> count--;                                        // number of active elements is decreased
    return *(circ_ptr -> tail);                                 // returning the value read
}


/************************************************************************
* Function to initialize the tx_buffer
*
* Input variables: CircBuf_t *circ_ptr
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
__STATIC_INLINE void circular_buffer_init(CircBuf_t *circ_ptr){
    circ_ptr-> buff = tx_buffer;
    circ_ptr-> head = circ_ptr-> buff;
    circ_ptr-> tail = circ_ptr-> buff;
}




#endif /* CIRCULAR_BUFFER_H */
