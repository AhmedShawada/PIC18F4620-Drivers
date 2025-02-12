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

volatile uint8 Timer3_OverFlow = 0;
uint16 Capture_Rising_Edge = 0;  
uint16 Capture_Falling_Edge = 0;
volatile uint8 CCP1_Second_Copture_Flag = 0;
uint32 Total_Period_MicroSecond = 0;
uint32 Freq = 0;
uint32 duty_cycle = 0;
uint32 High_Time = 0;



timer3_t timer3_obj =
{
    .TMR3_InterruptHandler = TMR3_DefaultInterruptHandler,
    .timer3_mode = TIMER3_TIMER_MODE,
    .priority = INTERRUPT_LOW_PRIORITY,
    .timer3_prescaler_value = TIMER3_PRESCALER_DIV_BY_1,
    .timer3_preload_value = 0,
    .timer3_reg_wr_mode = TIMER3_RW_REG_8Bit_MODE
};


ccp_t ccp1_obj = 
{
    .ccp_inst = CCP1_INST,
    .CCP1_InterruptHandler = CCP1_DefaultInterruptHandler,
    .ccp_mode = CCP_CAPTURE_MODE_SELECTED,
    .ccp_mode_variant = CCP_CAPTURE_MODE_1_RISING_EDGE,
	.ccp_capture_timer = CCP1_CCP2_TIMER3,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.pin = GPIO_PIN2,
    .ccp_pin.direction = GPIO_DIRECTION_INPUT
};



int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    while (1)
    { 
		if(2 == CCP1_Second_Copture_Flag)
		{
            Total_Period_MicroSecond = ((Timer3_OverFlow * 65536) + Capture_Falling_Edge);
			High_Time = Capture_Falling_Edge - Capture_Rising_Edge;	
            duty_cycle = (High_Time * 100) / Total_Period_MicroSecond;
			CCP1_Second_Copture_Flag = 0;
		}	
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = CCP_Init(&ccp1_obj);
    ret = Timer3_Init(&timer3_obj);
}

void CCP1_DefaultInterruptHandler(void)
{       
    std_ReturnType ret = E_NOT_OK;
    static uint8 CCP1_Interrupt_Flag = 0;
	CCP1_Interrupt_Flag++;
	
	if(1 == CCP1_Interrupt_Flag)
	{
		ret = CCP1_Capture_Mode_Read_Value(&ccp1_obj, &Capture_Rising_Edge);
        CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
	}
	
	else if(2 == CCP1_Interrupt_Flag)
	{
		ret = CCP1_Capture_Mode_Read_Value(&ccp1_obj, &Capture_Falling_Edge);
        CCP1_Second_Copture_Flag = 2;
		CCP1_Interrupt_Flag = 0;
		Timer3_OverFlow = 0;
	}
	
}  

void TMR3_DefaultInterruptHandler(void)
{
    Timer3_OverFlow++;
}