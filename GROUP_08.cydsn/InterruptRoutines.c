/* ========================================
 * InterruptRoutines.c file
 * author: Elena Stocco and Monica Loddo
 * Group 8 - Homework 3
*/
#include "InterruptRoutines.h"
#include "project.h"


uint8_t new_sample = 0;


CY_ISR(Custom_ISR_ADC){
    Timer_ReadStatusRegister();
    if(channel_0_ON){
        AMux_Select(CHANNEL_0_TEMP);
        ADC_DelSig_StartConvert();
        value_digit_Temp = ADC_DelSig_Read32();
        if (value_digit_Temp < 0) value_digit_Temp = 0;
        if (value_digit_Temp >65535) value_digit_Temp = 65535;
        value_mv_Temp= ADC_DelSig_CountsTo_mVolts(value_digit_Temp);
        ADC_DelSig_StopConvert();
	    PacketReadyFlagTemp=1;
    }
    if(channel_1_ON){
        AMux_Select(CHANNEL_1_LDR);
        ADC_DelSig_StartConvert();
        value_digit_LDR = ADC_DelSig_Read32();
        if (value_digit_LDR < 0) value_digit_LDR = 0;
        if (value_digit_LDR >65535) value_digit_LDR = 65535;
        value_mv_LDR= ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
        ADC_DelSig_StopConvert();
	    PacketReadyFlagLDR=1;
    }
}


void EZI2C_ISR_ExitCallback(void)
{
    //SWITCH ON LED
    if((channel_0_ON == 1) && (channel_1_ON == 1)){
        Blue_LED_Write(LED_ON);
    }
    else Blue_LED_Write(LED_OFF);
    
    
    //UPDATE CONTROL REGISTER 0
    if (slaveBuffer[0] != control_reg_0)
    {
        //UPDATE STATE
        if((slaveBuffer[0] & SELECT_BIT_0) == 1) channel_0_ON = 1;
        else channel_0_ON = 0;
        if((slaveBuffer[0] & SELECT_BIT_1) == 2) channel_1_ON = 1;
        else channel_1_ON = 0;
        
        //UPDATE NUMBER OF SAMPLES FOR AVERAGE (Bit 2-5)
        new_sample=(slaveBuffer[0] & SELECT_BIT_2_5)>>2;
        if(new_sample != samples) samples = new_sample;
        
        control_reg_0 = slaveBuffer[0];
        
    } 
    else
    {
        slaveBuffer[0] = control_reg_0;
    }
    
    
    
    //UPDATE CONTROL REGISTER 1
    if (slaveBuffer[1] != timer_period)
    {   
        Timer_Stop();
        Timer_WritePeriod(slaveBuffer[1]-1);
        Timer_WriteCounter(Timer_ReadPeriod());
        Timer_Start();
        timer_period = slaveBuffer[1];
    }
    else
    {
        slaveBuffer[1] = timer_period;
    }
}

/* [] END OF FILE */
