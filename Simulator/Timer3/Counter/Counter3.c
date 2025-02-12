/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on August 31, 2024, 12:32 AM
 */


#include "application.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer0/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"

volatile uint16 Counter = 0;

timer3_t timer3_counter_obj = 
{
    .TMR3_InterruptHandler = NULL,
    .priority = INTERRUPT_LOW_PRIORITY,
    .timer3_mode = TIMER3_COUNTER_MODE,
    .timer3_prescaler_value = TIMER3_PRESCALER_DIV_BY_1,
    .timer3_preload_value = 0,
    .timer3_reg_wr_mode = TIMER3_RW_REG_16Bit_MODE,
    .timer3_counter_mode = TIMER3_SYNC_COUNTER_MODE
};

int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    
 
    while (1)
    { 
        ret = Timer3_Read_Value(&timer3_counter_obj,&Counter);
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = Timer3_Init(&timer3_counter_obj);
    
    
}


