/* ========================================
 *
 *  main.c file
 *  

*/
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

//array contententi i 5 valori da mediare per i sude segnali: LDR e temp sensor
int32 LDR_values[5];
int32 Temp_values[5];

//variabili contententi la media
int32 average_LDR;
int32 average_Temp;

//ultimi valori campionati da ADC
extern volatile int32 value_mv_LDR;
extern volatile int32 value_mv_Temp;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_DelSig_Start();
    Timer_Start();
    AMux_Init(); //o AMux_Start??
    
    isr_ADC_StartEx(Custom_ISR_ADC);
    
    PacketReadyFlag=0;
    ADC_DelSig_StartConvert();
    
    //Calcolo il primo sample
    uint_t i=0;
    while (i<5){
        if (PacketReadyFlag == 1){
            LDR_values[i]=value_mv_LDR;
            Temp_values[i]=value_mv_Temp;
            average_LDR+=LDR_values[i];
            average_Temp+=Temp_values[i];
            i++;
            PacketReadyFlag = 0;
            }  
    }
    
    //calcolo media
    average_LDR\=5;
    FirstSampleReadyFlag = 1;
    SampleReadyFlag = 1;

    for(;;)
    {
        /* Place your application code here. */
        
        
    }
}

/* [] END OF FILE */
