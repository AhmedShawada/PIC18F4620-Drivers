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


void CCP1_DefaultInterruptHandler(void);
void TMR3_DefaultInterruptHandler(void);

ccp_t ccp_obj;
timer3_t timer3_obj;

void CCP1_DefaultInterruptHandler(void)
{       
    std_ReturnType ret = E_NOT_OK;
    ret = Timer3_Write_Value(&timer3_obj, 0);
	
}  

int main()
{
    std_ReturnType ret = E_NOT_OK;
  
    application_intialize();
    
    ccp_obj.CCP1_InterruptHandler = CCP1_DefaultInterruptHandler;
    ccp_obj.ccp_inst = CCP1_INST;
    ccp_obj.ccp_mode = CCP_COMPARE_MODE_SELECTED;
    ccp_obj.ccp_mode_variant = CCP_COMPARE_MODE_TOGGLE_ON_MATCH;
    ccp_obj.ccp_capture_timer = CCP1_CCP2_TIMER3;
    ccp_obj.ccp_pin.port = PORTC_INDEX;
    ccp_obj.ccp_pin.pin = GPIO_PIN2;
    ccp_obj.ccp_pin.direction = GPIO_DIRECTION_OUTPUT;
    
    ret = CCP_Compare_Mode_Set_Value(&ccp_obj, 50000);
    
    ret = CCP_Init(&ccp_obj);
    
    timer3_obj.TMR3_InterruptHandler = NULL;
    timer3_obj.timer3_mode = TIMER3_TIMER_MODE;
    timer3_obj.priority = INTERRUPT_LOW_PRIORITY;
    timer3_obj.timer3_prescaler_value = TIMER3_PRESCALER_DIV_BY_1;
    timer3_obj.timer3_preload_value = 0;
    timer3_obj.timer3_reg_wr_mode = TIMER3_RW_REG_16Bit_MODE;
    ret = Timer3_Init(&timer3_obj);
    
    
    while (1)
    { 
		
		
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
}


