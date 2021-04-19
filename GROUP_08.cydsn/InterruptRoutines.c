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

int32 value_digit_LDR;
int32 value_digit_Temp;
int32 value_mv_LDR;
int32 value_mv_Temp;
extern volatile int32 LDR_values[5];
extern volatile int32 Temp_values[5];

//canali del mux
#define LDR_CHANNEL 0;
#define TEMP_CHANNEL 1;


CY_ISR(Custom_ISR_ADC){
    // Read Timer status register to bring interrupt line low
    Timer_ReadStatusRegister();
    
    //connect the mux to ldr channel
    AMux_FastSelect(LDR_CHANNEL);
    value_digit_LDR = ADC_DelSig_Read32();
    if (value_digit_LDR < 0) value_digit_LDR = 0;
    if (value_digit_LDR >65535) value_digit_LDR = 65535;
    value_mv_LDR = ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
    //se ho già calcolato il primo sample, aggiorno il successivo
    if (FirstSampleReadyFlag == 1){
        LDR_values[1]=LDR_values[2];
        LDR_value[2]=LDR_values[3];
        LDR_values[3]=LDR_values[4];
        LDR_values[4]=LDR_values[5];
        LDR_values[5]=value_mv_LDR;
        SampleReadyFlag=1;
    }
    //sprintf(DataBuffer, "Sample: %ld mV\r\n", value_mv);
    
    //connect the mux to temp channel
    AMux_FastSelect(TEMP_CHANNEL);
    value_digit_LDR = ADC_DelSig_Read32();
    if (value_digit_Temp < 0) value_digit_Temp = 0;
    if (value_digit_Temp >65535) value_digit_Temp = 65535;
    value_mv_Temp = ADC_DelSig_CountsTo_mVolts(value_digit_Temp);
    if (FirstSampleReadyFlag == 1){
        Temp_values[1]=Temp_values[2];
        Temp_value[2]=Temp_values[3];
        Temp_values[3]=Temp_values[4];
        Temp_values[4]=Temp_values[5];
        Temp_values[5]=value_mv_Temp;
        SampleReadyFlag=1;
    }
    PacketReadyFlag = 1;
}

/* [] END OF FILE */
