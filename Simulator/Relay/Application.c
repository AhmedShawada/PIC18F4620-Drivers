/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"

#define _XTAL_FREQ 4000000 


relay_t relay_1 = 
    {
      .relay_port   = PORTC_INDEX,
      .relay_pin    = GPIO_PIN0,
      .relay_status = RELAY_OFF_STATUS 
    };

relay_t relay_2 = 
    {
      .relay_port   = PORTC_INDEX,
      .relay_pin    = GPIO_PIN1,
      .relay_status = RELAY_OFF_STATUS 
    };


   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();

    while(1)
    {
        ret = relay_turn_on(&relay_1);
        ret = relay_turn_off(&relay_2);
        __delay_ms(5000);
         ret = relay_turn_off(&relay_1);
         ret = relay_turn_on(&relay_2);
        __delay_ms(5000);
        
    }
    return (EXIT_SUCCESS);
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    
    ret = relay_intialize(&relay_1);
    ret = relay_intialize(&relay_2);
}

