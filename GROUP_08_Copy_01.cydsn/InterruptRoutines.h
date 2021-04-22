/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    #define TRANSMIT_BUFFER_SIZE 16
    #define SAMPLES 5
    #define LDR_SAMPLING 0
    #define TEMP_SAMPLING 1
    //LED
    #define HIGH 1
    #define LOW 0
    #define LED_ON HIGH
    #define LED_OFF LOW
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    char DataBuffer[TRANSMIT_BUFFER_SIZE];
    volatile uint8 PacketReadyFlagLDR;
    volatile uint8 PacketReadyFlagTemp;
    
#endif
    
    

/* [] END OF FILE */
