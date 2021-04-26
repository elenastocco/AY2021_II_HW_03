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
    #define INIT_CONTROL_REG_0 0xc0
    //LED
    #define HIGH 1
    #define LOW 0
    #define LED_ON HIGH
    #define LED_OFF LOW
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    //VARIABLE DEFINITIONS
    
  /*  volatile uint8 PacketReadyFlagLDR = 0;
    volatile uint8 PacketReadyFlagTemp = 0;
    volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
    volatile uint8_t control_reg_0 = 0x00;
    volatile uint8_t channel_0_ON = 0;
    volatile uint8_t channel_1_ON = 0;
    volatile int32 value_digit_Temp = 0;
    volatile int32 value_mv_Temp = 0;
    volatile int32 value_digit_LDR = 0;
    volatile int32 value_mv_LDR = 0;
    volatile uint8_t samples = 0;
    volatile int16 timer_period = 0;*/
    
#endif
    
    

/* [] END OF FILE */
