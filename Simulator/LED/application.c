/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 21, 2024, 11:07 PM
 */

#include "application.h"

#define _XTAL_FREQ 4000000 

led_t led1 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};
led_t led2 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN1, .led_status = GPIO_LOW};
led_t led3 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN2, .led_status = GPIO_LOW};
led_t led4 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN3, .led_status = GPIO_LOW};
led_t led5 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN4, .led_status = GPIO_LOW};
led_t led6 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN5, .led_status = GPIO_LOW};
led_t led7 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN6, .led_status = GPIO_LOW};
led_t led8 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN7, .led_status = GPIO_LOW};



    
int main()
{
    application_intialize();
   
    while(1)
    {
       
        led_turn_on(&led1);
        led_turn_off(&led2);
        led_turn_on(&led3);
        led_turn_off(&led4);
        led_turn_on(&led5);
        led_turn_off(&led6);
        led_turn_on(&led7);
        led_turn_off(&led8);
        
        __delay_ms(250);
        led_turn_off(&led1);
        led_turn_on(&led2);
        led_turn_off(&led3);
        led_turn_on(&led4);
        led_turn_off(&led5);
        led_turn_on(&led6);
        led_turn_off(&led7);
        led_turn_on(&led8); 
        
        __delay_ms(250);
        
        /*
        led_turn_toggle(&led1);
        led_turn_toggle(&led2);
        led_turn_toggle(&led3);
        led_turn_toggle(&led4);
        led_turn_toggle(&led5);
        led_turn_toggle(&led6);
        led_turn_toggle(&led7);
        led_turn_toggle(&led8);
        __delay_ms(250);
        */
        
        
    }
    
   
    
    return (EXIT_SUCCESS);
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = led_intialize(&led1);
    ret = led_intialize(&led2);
    ret = led_intialize(&led3);
    ret = led_intialize(&led4);
    ret = led_intialize(&led5);
    ret = led_intialize(&led6);
    ret = led_intialize(&led7);
    ret = led_intialize(&led8);

    
}

