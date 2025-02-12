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

button_t btn_high = {
    .button_pin.port = PORTC_INDEX,
    .button_pin.pin = GPIO_PIN2,
    .button_pin.logic = GPIO_LOW,
    .button_pin.direction = GPIO_DIRECTION_INPUT,
    .button_connection = BUTTON_ACTIVE_HIGH,
    .button_state = BUTTON_RELEASED     
};

button_state_t btn_high_status = BUTTON_RELEASED;
uint8 btn_high_status_flag = 0;
   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();

    while(1)
    {
        ret = button_read_state(&btn_high, &btn_high_status);
    
        if((BUTTON_PRESSED == btn_high_status)&&(0 == btn_high_status_flag))
        {
          led_turn_on(&led1);
        }
        else if((BUTTON_PRESSED == btn_high_status)&&(1 == btn_high_status_flag))
        {
          led_turn_off(&led1);
        }       
        
    }
    
   
    
    return (EXIT_SUCCESS);
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    
    ret = button_intialize(&btn_high);
    ret = led_intialize(&led1);
}

