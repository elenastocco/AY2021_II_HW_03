/* ========================================
 * -scrittura registri non funziona: capire come scrivergli con il 
 *  bridge control panel
 * -verifica cast che sia giusto e divisione in msb e lsb
 * -debug: come leggere variabili
 *  

*/
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"



//numero registri nello slave
#define SLAVE_BUFFER_SIZE 7

//definisco il buffer contenente i registri
uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];
uint8_t control_reg_0 = 0x14;
uint8_t channel_0_ON = 1;
uint8_t channel_1_ON = 0;

//variables definition
//int32 value_digit=0;
int32 value_digit_Temp=0;
//int32 value_mv=0;
int32 value_mv_Temp=0;
int32 value_digit_LDR=0;
int32 value_mv_LDR=0;
int count_LDR=0; 
int count_Temp=0;
int channel=0;
int status=LDR_SAMPLING;
int32 LDR_values[SAMPLES];
int32 Temp_values[SAMPLES];
int32 sum_LDR=0;
int16 mean_LDR=0;
int32 sum_temp=0;
int16 mean_temp=0;
uint8_t msb_LDR=0;
uint8_t lsb_LDR=0;
uint8_t msb_temp=0;
uint8_t lsb_temp=0;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_DelSig_Start();
    Timer_Start();
    AMux_Init(); //o AMux_Start??
    isr_ADC_StartEx(Custom_ISR_ADC);
    ADC_DelSig_StartConvert();
    UART_Start();
    UART_PutString("inizio \r\n");
    /* Start EZI2C Component */
    EZI2C_Start();

    //INIZIALIZZAZIONE REGISTRI
    //Control Register 0
    slaveBuffer[0] = control_reg_0; // dubbio: i primi due bit riservati vanno messi a 0?
    //Control Register 1: periodo del timer -> 4 ms, scrivo 4 (?)
    slaveBuffer[1] = Timer_ReadPeriod(); //4ms
    //Who Am I Register
    slaveBuffer[2] = 0xBC;
    //Ch0 Bit 15-8
    slaveBuffer[3] = 0x00;
    //Ch0 Bit 07-0
    slaveBuffer[4] = 0x00;
    //Ch1 Bit 15-8
    slaveBuffer[5] = 0x00;
    //Ch1 Bit 07-0
    slaveBuffer[6] = 0x00;
       
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 2 ,slaveBuffer);
    
    //sprintf(DataBuffer, "channel_0: %ld \r\n", channel_0_ON);
    //UART_PutString(DataBuffer);
    //sprintf(DataBuffer, "channel_1: %ld \r\n", channel_1_ON);
    //UART_PutString(DataBuffer);
    if((channel_0_ON == 1) && (channel_1_ON == 1)){
        Blue_LED_Write(LED_ON);
    }
    else Blue_LED_Write(LED_OFF);

    for(;;)
    
    { 
        if(channel_0_ON){
            if(PacketReadyFlagLDR==1){
            PacketReadyFlagLDR=0;
            count_LDR++;
            LDR_values[count_LDR]=value_mv_LDR;
            sprintf(DataBuffer, "sample_LDR: %ld mV\r\n", value_mv_LDR);
            UART_PutString(DataBuffer);
            sum_LDR+=LDR_values[count_LDR];
            mean_LDR=sum_LDR/5;   
            }
            
        
            //quando il count arriva a 5 ho già acquisito i 5 campioni LDR quindi posso passare al canale Temp
            if (count_LDR==5){
                count_LDR=0;
                sprintf(DataBuffer, "average_LDR: %ld mV\r\n", mean_LDR);
                UART_PutString(DataBuffer);
                PacketReadyFlagLDR=0;
                //scrivo la media nei registri 0x03 e 0x04
                /* la media è salvata in un int32, a me servono 16 bit quindi
                devo fare un cast a 16 bit, dividere i 16 bit in MSB e LSB e
                metterli nel registro
                */
                msb_LDR =(mean_LDR & 0xF0)>>8;
                lsb_LDR = (mean_LDR & 0x0F);
                slaveBuffer[3]=msb_LDR;
                slaveBuffer[4]=lsb_LDR;
                
                sum_LDR=0;
                mean_LDR=0;   
            }
                
            }
        if(channel_1_ON){
            if(PacketReadyFlagTemp==1){
                PacketReadyFlagTemp=0;
                count_Temp++;
                Temp_values[count_Temp]=value_mv_Temp;
                sprintf(DataBuffer, "sample_Temp: %ld mV\r\n", value_mv_Temp);
                UART_PutString(DataBuffer);
                sum_temp+=Temp_values[count_Temp];
                mean_temp=sum_temp/5;
            }
                
                //se il count è a 5, ho già i 5 samples e posso cambiare stato
                if (count_Temp==5){
                    count_Temp=0;
                    sprintf(DataBuffer, "average_Temp: %ld mV\r\n", mean_temp);
                    UART_PutString(DataBuffer);
                    PacketReadyFlagTemp=0;
                    //scrittura registri
                    msb_temp =(mean_temp & 0xF0)>>8;
                    lsb_temp = (mean_temp & 0x0F);
                    slaveBuffer[3]=msb_temp;
                    slaveBuffer[4]=lsb_temp;
                    
                    sum_temp=0;
                    mean_temp=0;
                }
            
        }
        
}
}


/* [] END OF FILE */
