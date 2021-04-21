/* ========================================
 *
 *  main.c file
 *  

*/
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

//numero registri nello slave
#define SLAVE_BUFFER_SIZE 7
//definisco il buffer contenente i registri
uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];

/* Start EZI2C Component */
EZI2C_Start();

//INIZIALIZZAZIONE REGISTRI
//Control Register 0
slaveBuffer[0] = 0x14; // dubbio: i primi due bit riservati vanno messi a 0?
//Control Register 1: periodo del timer -> 4 ms, scrivo 4 (?)
slaveBuffer[1] = 0x04; //4ms
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
EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1 ,slaveBuffer);

//variables definition
int32 value_digit=0;
//int32 value_digit_Temp=0;
int32 value_mv=0;
//int32 value_mv_Temp=0;
int count_LDR=0; 
int count_Temp=0;
int channel=0;
int status=LDR_SAMPLING;
int32 LDR_values[SAMPLES];
int32 Temp_values[SAMPLES];
int32 sum_LDR=0;
int32 mean_LDR=0;
int32 sum_temp=0;
int32 mean_temp=0;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_DelSig_Start();
    Timer_Start();
    AMux_Init(); //o AMux_Start??
    isr_ADC_StartEx(Custom_ISR_ADC);
    PacketReadyFlag=0;
    ADC_DelSig_StartConvert();
   

    for(;;)
    
    { 
        switch (status){
        //nello stato zero sto facendo calcolare la media dei primi 5 samples campionati dal canale LDR
        case LDR_SAMPLING:
        AMux_FastSelect(channel);
        if (PacketReadyFlag==1){
            count_LDR++;
            LDR_values[count_LDR]=value_mv;
            sum_LDR+=LDR_values[count_LDR];
            mean_LDR=sum_LDR/5;
            
        }
        
       //quando il count arriva a 5 ho già acquisito i 5 campioni LDR quindi posso passare al canale Temp
        if (count_LDR==5){
            count_LDR=0;
            channel=1;
            status=TEMP_SAMPLING;
            PacketReadyFlag=0; 
        }
        break;
        
        //calcolo la media dei primi 5 campioni del canale Temp
        case TEMP_SAMPLING:
        AMux_FastSelect(channel);
        if (PacketReadyFlag==1){
            PacketReadyFlag=0;
            count_Temp++;
            Temp_values[count_Temp]=value_mv;
            sum_temp+=Temp_values[count_Temp];
            mean_temp=sum_temp/5;
        }
        //se il count è a 5, ho già i 5 samples e posso cambiare stato
        if (count_Temp==5){
            count_Temp=0;
            channel=0;
            PacketReadyFlag=0;
            status=LDR_SAMPLING;
        }
        break;
        }
    }
}


/* [] END OF FILE */
