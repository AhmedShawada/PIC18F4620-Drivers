#include "application.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer0/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"
#include "MCAL_Layer/CCP/hal_ccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/I2C/hal_i2c.h"

uint8 Temp_Value;


int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
   
    
    while (1)
    { 
        ReadTEMPSENSOR_TC74(0x9E,&Temp_Value);
    }
    
    return (EXIT_SUCCESS);
}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK; 
    
    
    
}




