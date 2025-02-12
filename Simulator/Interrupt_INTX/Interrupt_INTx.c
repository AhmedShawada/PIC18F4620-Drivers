/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on August 31, 2024, 12:32 AM
 */


#include "application.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"


led_t led1 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};
led_t led2 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN1, .led_status = GPIO_LOW};
led_t led3 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN2, .led_status = GPIO_LOW};



void INT0_Int_ISR(void)
{
    led_turn_on(&led1);
}

void INT1_Int_ISR(void)
{
    led_turn_on(&led2);
}

void INT2_Int_ISR(void)
{
    led_turn_on(&led3);
}




interrupt_INTx_t INt0_int_obj = 
{
    .EXT_InterruptHandler = INT0_Int_ISR,
    .edge = INTERRUPT_RISING_EDGE,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .source = INTERRUPT_EXTERNAL_INT0,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT,
    .mcu_pin.pin = GPIO_PIN0,
    .mcu_pin.port = PORTB_INDEX, 
};

interrupt_INTx_t INt1_int_obj = 
{
    .EXT_InterruptHandler = INT1_Int_ISR,
    .edge = INTERRUPT_FALLING_EDGE,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .source = INTERRUPT_EXTERNAL_INT1,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT,
    .mcu_pin.pin = GPIO_PIN1,
    .mcu_pin.port = PORTB_INDEX, 
};

interrupt_INTx_t INt2_int_obj = 
{
    .EXT_InterruptHandler = INT2_Int_ISR,
    .edge = INTERRUPT_RISING_EDGE,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .source = INTERRUPT_EXTERNAL_INT2,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT,
    .mcu_pin.pin = GPIO_PIN2,
    .mcu_pin.port = PORTB_INDEX, 
};


void main()
{
    std_ReturnType ret = E_NOT_OK;
    
    application_intialize();

    while (1)
    {
        
    }
}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    
    ret = led_intialize(&led1);
    ret = led_intialize(&led2);
    ret = led_intialize(&led3);
    
    
    
    
    ret = Interrupt_INTx_Init(&INt0_int_obj);
    ret = Interrupt_INTx_Init(&INt1_int_obj);
    ret = Interrupt_INTx_Init(&INt2_int_obj);
    
    
    
}
