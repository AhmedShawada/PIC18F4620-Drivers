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
led_t led4 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN3, .led_status = GPIO_LOW};


void RB4_HIGH_Int_ISR(void) 
{
    led_turn_on(&led1);
}

void RB4_LOW_Int_ISR(void) 
{
    led_turn_on(&led2);
}

void RB5_HIGH_Int_ISR(void) 
{
    led_turn_off(&led1);
}

void RB5_LOW_Int_ISR(void) 
{
    led_turn_off(&led2);
}



void RB6_HIGH_Int_ISR(void) 
{
    led_turn_on(&led3);
}

void RB6_LOW_Int_ISR(void) 
{
    led_turn_on(&led4);
}

void RB7_HIGH_Int_ISR(void) 
{
    led_turn_off(&led3);
}

void RB7_LOW_Int_ISR(void) 
{
    led_turn_off(&led4);
}




interrupt_RBx_t rb4_int_obj =
{
    .EXT_InterruptHandler_HIGH = RB4_HIGH_Int_ISR,
    .EXT_InterruptHandler_LOW  = RB4_LOW_Int_ISR, 
    .priority = INTERRUPT_HIGH_PRIORITY,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN4,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT 
};

interrupt_RBx_t rb5_int_obj =
{
    .EXT_InterruptHandler_HIGH = RB5_HIGH_Int_ISR,
    .EXT_InterruptHandler_LOW  = RB5_LOW_Int_ISR, 
    .priority = INTERRUPT_HIGH_PRIORITY,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN5,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT 
};


interrupt_RBx_t rb6_int_obj =
{
    .EXT_InterruptHandler_HIGH = RB6_HIGH_Int_ISR,
    .EXT_InterruptHandler_LOW  = RB6_LOW_Int_ISR, 
    .priority = INTERRUPT_HIGH_PRIORITY,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN6,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT 
};

interrupt_RBx_t rb7_int_obj =
{
    .EXT_InterruptHandler_HIGH = RB7_HIGH_Int_ISR,
    .EXT_InterruptHandler_LOW  = RB7_LOW_Int_ISR, 
    .priority = INTERRUPT_HIGH_PRIORITY,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN7,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT 
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
    ret = led_intialize(&led4);
    
    
    ret = Interrupt_RBx_Init(&rb4_int_obj);
    ret = Interrupt_RBx_Init(&rb5_int_obj);
    ret = Interrupt_RBx_Init(&rb6_int_obj);
    ret = Interrupt_RBx_Init(&rb7_int_obj);
    
    
    
    
    
}
