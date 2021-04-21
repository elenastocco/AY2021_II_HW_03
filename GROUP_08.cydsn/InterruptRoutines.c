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
#include "InterruptRoutines.h"
#include "project.h"

extern volatile int32 value_digit;
//extern volatile int32 value_digit_Temp;
extern volatile int32 value_mv;
//extern volatile int32 value_mv_Temp;

extern volatile int count_LDR;
extern volatile int count_Temp;



CY_ISR(Custom_ISR_ADC){
    // Read Timer status register to bring interrupt line low
    Timer_ReadStatusRegister();
    ADC_DelSig_StartConvert();
    value_digit = ADC_DelSig_Read32();
    if (value_digit < 0) value_digit = 0;
    if (value_digit >65535) value_digit = 65535;
    value_mv= ADC_DelSig_CountsTo_mVolts(value_digit);
    ADC_DelSig_StopConvert();
    //UART_PutString(DataBuffer);
	PacketReadyFlag=1;
	}

/* [] END OF FILE */
