/* ========================================
 * InterruptRoutines.h file
 * author: Elena Stocco and Monica Loddo
 * Group 8 - Homework 3
*/

#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    #define SLAVE_BUFFER_SIZE 7
    #define CONTROL_REG_0_INIT 0x00
    #define CHANNEL_0_TEMP 0 
    #define CHANNEL_1_LDR 1
    
    //MASKS to select bit in Control Register 0
    #define SELECT_BIT_0 0x01
    #define SELECT_BIT_1 0x02
    #define SELECT_BIT_2_5 0x3C
    
    //LED
    #define HIGH 1
    #define LOW 0
    #define LED_ON HIGH
    #define LED_OFF LOW
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    
  
    
#endif
    
    

/* [] END OF FILE */
