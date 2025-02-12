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


volatile uint32 timer3_on;
led_t led1 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};

void Timer_InterruptHandler(void)
{
    timer3_on++;
    led_turn_toggle(&led1);
}


timer3_t timer3_timer_obj = 
{    
    .TMR3_InterruptHandler = Timer_InterruptHandler,
    .priority = INTERRUPT_LOW_PRIORITY,
    .timer3_mode = TIMER3_TIMER_MODE,
    .timer3_prescaler_value = TIMER3_PRESCALER_DIV_BY_8,
    .timer3_preload_value = 15536,
    .timer3_reg_wr_mode = TIMER3_RW_REG_16Bit_MODE,
};


int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    

    while (1)
    { 
        
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = Timer3_Init(&timer3_timer_obj);
    ret = led_intialize(&led1);
    
}


