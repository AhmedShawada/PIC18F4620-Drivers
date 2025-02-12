/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"

uint8 keypad_value = ZERO_INIT;

keypad_t keypad = 
{
  .keypad_row_pins[0].port = PORTC_INDEX, 
  .keypad_row_pins[0].pin = GPIO_PIN0,
  .keypad_row_pins[0].logic = GPIO_LOW,
  .keypad_row_pins[0].direction = GPIO_DIRECTION_OUTPUT,
  
  .keypad_row_pins[1].port = PORTC_INDEX, 
  .keypad_row_pins[1].pin = GPIO_PIN1,
  .keypad_row_pins[1].logic = GPIO_LOW,
  .keypad_row_pins[1].direction = GPIO_DIRECTION_OUTPUT,
  
  .keypad_row_pins[2].port = PORTC_INDEX, 
  .keypad_row_pins[2].pin = GPIO_PIN2,
  .keypad_row_pins[2].logic = GPIO_LOW,
  .keypad_row_pins[2].direction = GPIO_DIRECTION_OUTPUT,
  
  .keypad_row_pins[3].port = PORTC_INDEX, 
  .keypad_row_pins[3].pin = GPIO_PIN3,
  .keypad_row_pins[3].logic = GPIO_LOW,
  .keypad_row_pins[3].direction = GPIO_DIRECTION_OUTPUT,
  
  .keypad_columns_pins[0].port = PORTC_INDEX,
  .keypad_columns_pins[0].pin = GPIO_PIN4,
  .keypad_columns_pins[0].logic = GPIO_LOW,
  .keypad_columns_pins[0].direction = GPIO_DIRECTION_INPUT,
  
  .keypad_columns_pins[1].port = PORTC_INDEX,
  .keypad_columns_pins[1].pin = GPIO_PIN5,
  .keypad_columns_pins[1].logic = GPIO_LOW,
  .keypad_columns_pins[1].direction = GPIO_DIRECTION_INPUT,
  
  .keypad_columns_pins[2].port = PORTC_INDEX,
  .keypad_columns_pins[2].pin = GPIO_PIN6,
  .keypad_columns_pins[2].logic = GPIO_LOW,
  .keypad_columns_pins[2].direction = GPIO_DIRECTION_INPUT,
  
  .keypad_columns_pins[3].port = PORTC_INDEX,
  .keypad_columns_pins[3].pin = GPIO_PIN7,
  .keypad_columns_pins[3].logic = GPIO_LOW,
  .keypad_columns_pins[3].direction = GPIO_DIRECTION_INPUT,
};

led_t led1 = {.port_name = PORTD_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};


   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    while(1)
    {
       ret = keypad_get_value(&keypad,&keypad_value);
       
       if('8' == keypad_value)
       {
         led_turn_on(&led1);   
       }
       
       else if('9' == keypad_value)
       {
         led_turn_off(&led1);  
       }
   
    } 
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = keypad_intialize(&keypad);
    ret = led_intialize(&led1);
    
}

