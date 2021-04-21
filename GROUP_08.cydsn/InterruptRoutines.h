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
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    char DataBuffer[TRANSMIT_BUFFER_SIZE];
    volatile uint8 PacketReadyFlag;
    
#endif
    
    

/* [] END OF FILE */
