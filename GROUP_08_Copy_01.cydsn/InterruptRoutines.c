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

#define CONTROL_REG_0_INIT 0x14
#define SLAVE_BUFFER_SIZE 7

extern volatile int32 value_digit_LDR;
extern volatile int32 value_digit_Temp;
extern volatile int32 value_mv_LDR;
extern volatile int32 value_mv_Temp;

extern volatile int count_LDR;
extern volatile int count_Temp;
extern volatile uint8_t control_reg_0;
extern volatile uint8_t channel_0_ON;
extern volatile uint8_t channel_1_ON;
extern volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];

/* mandare la media alla frequenza di 50 Hz: se la frequenza dell'isr è 
*  250 Hz, ogni 5 esecuzioni di ISR (ovvero ogni volta che ho calcolato la media),
   devo scrivere nei registro ad indirizzo 0x03, 0x04, 0x05 e 0x06 le medie dei due canali.
   domanda: se devo capionare un solo canale, nell'altro scrivo 0 come media o lascio il valore vecchio?
   metto nell'isr una flag, che mi faccia entrare in quella parte di codice ogni qual volta che una media è
   pronta, ovvero ogni 5 campionamenti, in questo codice scrivo nei registri, 
   dal bridge control panel dovrei essere in grado di leggere tali registri.
   dubbio: come faccio a leggere questi registri in 'real time' dato che cambiano ogni 50Hz?? devo plottare qualcosa?
   dubbio: la media l'ho salvata in un int32, però qui devo salvare in 16, divisa in 8 MSB e 8 LSB, come fare
*/

CY_ISR(Custom_ISR_ADC){
    // Read Timer status register to bring interrupt line low
    if(channel_0_ON){
        AMux_Start();
        AMux_FastSelect(0);
        Timer_ReadStatusRegister();
        ADC_DelSig_StartConvert();
        value_digit_LDR = ADC_DelSig_Read32();
        if (value_digit_LDR < 0) value_digit_LDR = 0;
        if (value_digit_LDR >65535) value_digit_LDR = 65535;
        value_mv_LDR= ADC_DelSig_CountsTo_mVolts(value_digit_LDR);
        ADC_DelSig_StopConvert();
        AMux_Stop();
	    PacketReadyFlagLDR=1;
    }
    if(channel_1_ON){
        AMux_Start();
        AMux_FastSelect(1);
        Timer_ReadStatusRegister();
        ADC_DelSig_StartConvert();
        value_digit_Temp = ADC_DelSig_Read32();
        if (value_digit_Temp < 0) value_digit_Temp = 0;
        if (value_digit_Temp >65535) value_digit_Temp = 65535;
        value_mv_Temp= ADC_DelSig_CountsTo_mVolts(value_digit_Temp);
        ADC_DelSig_StopConvert();
        AMux_Stop();
	    PacketReadyFlagTemp=1;
    }
}

//questa isr viene chiamata ogni volta che c'è stato un trasferimento di dati
//nel nostro caso, quando dal Bridge Control Panel voglio cambiare lo stato
void EZI2C_ISR_ExitCallback(void)
{
    //update control register if it was updated
    /*if (slaveBuffer[0] != control_reg_0)
    {
        //lo stato è cambiato, devo cambiare il canale
        //se il bit del canale 0 è a 1 (bit meno significativo), attivo canale 0
        if((slaveBuffer[0] & 0x01) == 1) channel_0_ON = 1;
        else channel_0_ON = 0;
        //se il bit del canale 1 è a 1 (secondo bit meno significativo), attivo canale 1
        if((slaveBuffer[0] & 0x02) == 2) channel_1_ON = 1;
        else channel_1_ON = 0;
        
        control_reg_0 = slaveBuffer[0];
        
    }
    */
    if(slaveBuffer[0] != control_reg_0){
        int channel_select = (slaveBuffer[0] & 0x03);
        if (channel_select == 0){
            channel_0_ON = 0;
            channel_1_ON = 0;
        }
        if (channel_select == 1){
            channel_0_ON = 1;
            channel_1_ON = 0;
        }
        if (channel_select == 2){
            channel_0_ON = 0;
            channel_1_ON = 1;
        }
        if (channel_select == 3){
            channel_0_ON = 1;
            channel_1_ON = 1;
        }
        control_reg_0 = slaveBuffer[0];
        
    }
    //se entrambi i canali sono attivati, accendo il LED
    /*if((channel_0_ON == 1) && (channel_1_ON == 1)){
        Blue_LED_Write(LED_ON);
    }
    else Blue_LED_Write(LED_OFF);
    */
    if((control_reg_0 & 0x03) == 3){
        Blue_LED_Write(LED_ON);
    }
    else Blue_LED_Write(LED_OFF);
     
}

/* [] END OF FILE */
