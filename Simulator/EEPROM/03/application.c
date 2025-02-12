/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on August 31, 2024, 12:32 AM
 */


#include "application.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"

led_t led = {.port_name = PORTC_INDEX ,.pin = GPIO_PIN0 ,.led_status = GPIO_LOW};

void main()
{
    std_ReturnType ret = E_NOT_OK;
    uint8 Value = 0;
    uint8 Read_Value;
    
    application_intialize();

    
    while (1)
    {
        ret = Data_EEPROM_WriteByte(0x23D,Value++);
        __delay_ms(500);
        ret = Data_EEPROM_ReadByte(0x23D,&Read_Value);
        if(3 == Read_Value)
        {
            led_turn_on(&led);
        }
        else
        {
            led_turn_off(&led);
        }
    }
}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = led_intialize(&led);

    
}
