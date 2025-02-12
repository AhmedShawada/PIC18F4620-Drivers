/* 
 * File:   UltraSonic.c
 * Author: ahmed-abdallah
 *
 * Created on November 7, 2024, 5:43 PM
 */


#include "UltraSonic.h" 

static void TMR1_Initialize(void);


std_ReturnType Ultrasonic_Init(const ultrasonic_t *_config)
{
    std_ReturnType ret = E_NOT_OK;
    
    
    
    if(NULL == _config)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = gpio_pin_intialize(&(_config->Trigger_Pin));
        ret &= gpio_pin_intialize(&(_config->Echo_Pin));
        
        ret = E_OK;  
    }
    
    return ret;
}
std_ReturnType Ultrasonic_Calculate_Distance(const ultrasonic_t *_config, uint16 *Distance)
{
    std_ReturnType ret = E_NOT_OK;
    uint16 Timer1_Value = 0;
    logic_t EchoPinLogic = GPIO_LOW;
    TMR1_Initialize();
    
    if((NULL == _config) || (NULL == Distance))
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Step (1) : Clear Timer 1 Register */
        ret = Timer1_Write_Value(&timer1_timer_obj,0);
        
        /* Step (2) : Send High Signal to the Ultrasonic Trigger Pin */
        ret = gpio_pin_write_logic(&(_config->Trigger_Pin), GPIO_HIGH);
        
        /* Step (2) : Wait 10 Micro Second */
        __delay_us(10);
        
        /* Step (3) : Send Low Signal to the Ultrasonic Trigger Pin */
        ret = gpio_pin_write_logic(&(_config->Trigger_Pin), GPIO_LOW);
        
        /* Step (4) : Check the Echo Pin to be High */
        while(GPIO_LOW == EchoPinLogic)
        {
            ret = gpio_pin_read_logic(&(_config->Echo_Pin), &EchoPinLogic);
        }
        
        /* Step (5) : Enable TMR1 Module */
        ret = Timer1_Init(&timer1_timer_obj);
        
        /* Step (6) : Check the Echo Pin to be Low */
        while(GPIO_HIGH == EchoPinLogic)
        {
            ret = gpio_pin_read_logic(&(_config->Echo_Pin), &EchoPinLogic);
        }
        
        /* Step (7) : Disable TMR1 Module */
        TIMER1_MODULE_DISABLE();
        
        /* Step (8) : Read the time */
        ret = Timer1_Read_Value(&timer1_timer_obj,&Timer1_Value);
        
        /* Step (9) : Calculate the distance */
        *Distance = (uint16)(Timer1_Value / 57);
   
        ret = E_OK;  
    }
    
    return ret;
}


static void TMR1_Initialize(void)
{
    std_ReturnType ret = E_NOT_OK;

    timer1_timer_obj.timer1_mode = TIMER1_TIMER_MODE;
    timer1_timer_obj.timer1_preload_value = 65528;
    timer1_timer_obj.timer1_prescaler_value = TIMER1_PRESCALER_DIV_BY_2;
    timer1_timer_obj.timer1_reg_wr_mode = TIMER1_RW_REG_16Bit_MODE;
    
    //ret = Timer1_Init(&timer1_timer_obj);
}