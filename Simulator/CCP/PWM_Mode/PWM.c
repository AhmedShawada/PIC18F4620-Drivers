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
#include "MCAL_Layer/CCP/hal_ccp.h"
#include "MCAL_Layer/CCP/hal_ccp.h"

uint8 CCP1_PWM1_Duty = 0;
timer2_t timer2_obj =
{
    .TMR2_InterruptHandler = NULL,
    .timer2_prescaler_value = TIMER2_PRESCALER_DIV_BY_1,
    .timer2_postscaler_value = TIMER2_POSTSCALER_DIV_BY_1,
    .timer2_preload_value = 0,
};


ccp_t ccp1_obj = 
{
    .ccp_inst = CCP1_INST,
    .CCP1_InterruptHandler = NULL,
    .ccp_mode = CCP_PWM_MODE_SELECTED,
    .PWM_Frequency = 20000,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.pin = GPIO_PIN2,
    .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
    .timer2_prescaler_value = CCP_TIMER2_PRESCALER_DIV_BY_1,
    .timer2_postscaler_value = CCP_TIMER2_POSTSCALER_DIV_BY_1,
};

ccp_t ccp2_obj =
{
    .ccp_inst = CCP2_INST,
    .CCP1_InterruptHandler = NULL,
    .ccp_mode = CCP_PWM_MODE_SELECTED,
    .PWM_Frequency = 20000,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.pin = GPIO_PIN1,
    .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
    .timer2_prescaler_value = CCP_TIMER2_PRESCALER_DIV_BY_1,
    .timer2_postscaler_value = CCP_TIMER2_POSTSCALER_DIV_BY_1
};

int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    ret = CCP_PWM_Set_Duty(&ccp2_obj, 25);
    
    ret = CCP_PWM_Start(&ccp1_obj);
    ret = CCP_PWM_Start(&ccp2_obj);

    while (1)
    { 
        for(CCP1_PWM1_Duty = 0; CCP1_PWM1_Duty < 100; CCP1_PWM1_Duty +=5)
        {
            __delay_ms(5);
            ret = CCP_PWM_Set_Duty(&ccp1_obj, CCP1_PWM1_Duty);
        }
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = CCP_Init(&ccp1_obj);
    ret = CCP_Init(&ccp2_obj);
    ret = Timer2_Init(&timer2_obj);
}