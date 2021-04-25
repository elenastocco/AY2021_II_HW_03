/* ========================================
 * main.c file
 * author: Elena Stocco and Monica Loddo
 * Group 8 - Homework 3
*/
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

//VARIABLE SETTINGS
int32 LDR_values = 0;
int32 Temp_values = 0;
int32 sum_LDR = 0;
int16 mean_LDR = 0;
uint16 mean_temp = 0;
int32 sum_temp = 0;
int count_LDR = 0; 
int count_Temp = 0;
uint8_t msb_LDR = 0;
uint8_t lsb_LDR = 0;
uint8_t msb_temp = 0;
uint8_t lsb_temp = 0;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_DelSig_Start();
    Timer_Start();
    AMux_Init();
    isr_ADC_StartEx(Custom_ISR_ADC);
    ADC_DelSig_StartConvert();
    EZI2C_Start();
    
    timer_period = Timer_ReadCounter();
    
    //REGISTERS INITIALIZATION
    slaveBuffer[0] = CONTROL_REG_0_INIT; //Control Register 0
    slaveBuffer[1] = timer_period; //Control Register 1
    slaveBuffer[2] = 0xBC; //Who Am I Register
    slaveBuffer[3] = 0x00;//Ch0 Bit 15-8
    slaveBuffer[4] = 0x00;//Ch0 Bit 07-0
    slaveBuffer[5] = 0x00;//Ch1 Bit 15-8
    slaveBuffer[6] = 0x00;//Ch1 Bit 07-0
       
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 2 ,slaveBuffer);

    for(;;)
    
    { 
        if(channel_0_ON){
            while (PacketReadyFlagTemp == 0){}
            Temp_values += value_mv_Temp;
            PacketReadyFlagTemp = 0;
            count_Temp++;
         
            if (count_Temp == samples){
                count_Temp = 0;
                mean_temp = Temp_values/samples;
                msb_temp = mean_temp>>8;
                lsb_temp = (mean_temp & 0xFF);
                //Registers writing
                slaveBuffer[3] = msb_temp;
                slaveBuffer[4] = lsb_temp;
                
                Temp_values = 0;
                mean_temp = 0;   
            }
                
        }
        if(channel_1_ON){
            while (PacketReadyFlagLDR == 0){}
            PacketReadyFlagLDR = 0;
            LDR_values += value_mv_LDR;
            count_LDR++;   
            
            if (count_LDR == samples){
                count_LDR=0;
                mean_LDR=LDR_values/samples;
                msb_LDR =mean_LDR>>8;
                lsb_LDR = (mean_LDR & 0xFF);
                //Registers writing
                slaveBuffer[5]=msb_LDR;
                slaveBuffer[6]=lsb_LDR;
                
                LDR_values=0;
                mean_LDR=0;   
            }
                
        }       
    }
}
/* [] END OF FILE */
