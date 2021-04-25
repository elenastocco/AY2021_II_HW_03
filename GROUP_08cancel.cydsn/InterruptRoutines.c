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

extern volatile int32 value_digit;
//extern volatile int32 value_digit_Temp;
extern volatile int32 value_mv;
//extern volatile int32 value_mv_Temp;

extern volatile int count_LDR;
extern volatile int count_Temp;
extern volatile uint8_t control_reg_0;
extern volatile uint8_t channel_0_ON;
extern volatile uint8_t channel_1_ON;
extern volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE];



CY_ISR(Custom_ISR_ADC){
    // Read Timer status register to bring interrupt line low
    Timer_ReadStatusRegister();
    ADC_DelSig_StartConvert();
    value_digit = ADC_DelSig_Read32();
    if (value_digit < 0) value_digit = 0;
    if (value_digit >65535) value_digit = 65535;
    value_mv= ADC_DelSig_CountsTo_mVolts(value_digit);
    ADC_DelSig_StopConvert();
    sprintf(DataBuffer, "sample: %ld mV\r\n", value_mv);
    UART_PutString(DataBuffer);
	PacketReadyFlag=1;
	}

//questa isr viene chiamata ogni votla che c'è stato un trasferimento di dati
//nel nostro caso, quando dal Bridge Control Panel voglio cambiare lo stato
void EZI2C_ISR_ExitCallback(void)
{
    // Update PWM compare value if it was updated and it is lower than period value
    if (slaveBuffer[0] != control_reg_0)
    {
        //lo stato è cambiato, devo cambiare il canale
        //se il bit del canale 0 è a 1, attivo canale 1
        if((control_reg_0) & (0x01 == 1)) channel_0_ON = 1;
        else channel_0_ON = 0;
        
        if((control_reg_0) & (0x02 == 1)) channel_1_ON = 1;
        else channel_1_ON = 1;
        
        control_reg_0 = slaveBuffer[0];
    }
    
}

/* [] END OF FILE */
