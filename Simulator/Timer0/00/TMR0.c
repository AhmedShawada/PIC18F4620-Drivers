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

volatile uint16 Counter = 0;
led_t led1 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};

void Timer_InterruptHandler(void)
{
    led_turn_toggle(&led1);
}


timer0_t timer0_timer_obj = 
{
    .TMR0_InterruptHandler = Timer_InterruptHandler,
    .prescaler_enable = TIMER0_PRESCALER_ENABLE_CFG,
    .prescaler_value = TIMER0_PRESCALER_DIV_BY_4,
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_register_size = TIMER0_16BIT_REGISTER_MODE,
    .timer0_preload_value = 3036,
};

timer0_t timer0_counter_obj = 
{
    .TMR0_InterruptHandler = Timer_InterruptHandler,
    .prescaler_enable = TIMER0_PRESCALER_DISABLE_CFG,
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_counter_edge = TIMER0_COUNTER_RISING_EDGE_CFG,
    .timer0_preload_value = 0
};


int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    
 
    while (1)
    { 
        ret = Timer0_Read_Value(&timer0_counter_obj,&Counter);
 
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    //ret = Timer0_Init(&timer0_timer_obj);
    ret = Timer0_Init(&timer0_counter_obj);
    ret = led_intialize(&led1);
    
}


