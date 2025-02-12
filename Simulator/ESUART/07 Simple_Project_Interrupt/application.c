/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on Oct 21, 2024, 6:32 PM
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
#include "MCAL_Layer/USART/hal_usart.h"

led_t led1 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};
led_t led2 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN1, .led_status = GPIO_LOW};

void EUSART_TxDefaultInterruptHandler(void);
void EUSART_RxDefaultInterruptHandler(void);
void EUSART_FramingErrorHandler (void);
void EUSART_OverrunErrorHandler (void);

uint8 RX_USART_REG;




void usart_module_init(void)
{
    usart_t usart_obj;
    std_ReturnType ret = E_NOT_OK;
    
    
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_gen_gonfig = BAUDRATE_ASYN_8BIT_lOW_SPEED;
    
    usart_obj.usart_tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUS_TX_ENABLE;
    usart_obj.usart_tx_cfg.usart_tx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE;
    usart_obj.usart_tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE;
    
    usart_obj.usart_rx_cfg.usart_rx_enable = EUSART_ASYNCHRONOUS_RX_ENABLE;
    usart_obj.usart_rx_cfg.usart_rx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE;
    usart_obj.usart_rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE;
    
    usart_obj.EUSART_FramingErrorHandler = NULL;
    usart_obj.EUSART_OverrunErrorHandler = NULL;
    usart_obj.EUSART_RxDefaultInterruptHandler = NULL;
    usart_obj.EUSART_TxDefaultInterruptHandler = NULL;
    
    ret = EUSART_ASYNC_Init(&usart_obj);
}

void usart_module_interrupt_init(void)
{
    usart_t usart_obj;
    std_ReturnType ret = E_NOT_OK;
    
    
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_gen_gonfig = BAUDRATE_ASYN_8BIT_lOW_SPEED;
    
    usart_obj.usart_tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUS_TX_ENABLE;
    usart_obj.usart_tx_cfg.usart_tx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE;
    usart_obj.usart_tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE;
    
    usart_obj.usart_rx_cfg.usart_rx_enable = EUSART_ASYNCHRONOUS_RX_ENABLE;
    usart_obj.usart_rx_cfg.usart_rx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE;
    usart_obj.usart_rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE;
    
    usart_obj.EUSART_TxDefaultInterruptHandler = EUSART_TxDefaultInterruptHandler;
    usart_obj.EUSART_RxDefaultInterruptHandler = EUSART_RxDefaultInterruptHandler;
    usart_obj.EUSART_FramingErrorHandler = NULL;
    usart_obj.EUSART_OverrunErrorHandler = NULL;
    
    ret = EUSART_ASYNC_Init(&usart_obj);
}

int main()
{
    std_ReturnType ret = E_NOT_OK;
  
    application_intialize();
    
    usart_module_interrupt_init();
    
    
    
    

    while (1)
    { 
//       ret = EUSART_ASYNC_WriteStringBlocking("Hello\r",6);
        
//        ret = EUSART_ASYNC_WriteByteBlocking('A');
//        __delay_ms(1000);
//        ret = EUSART_ASYNC_WriteByteBlocking('B');
//        __delay_ms(1000);
		
//		ret = EUSART_ASYNC_ReadByteNonBlocking(&RX_USART_REG);
//        
//        if(E_OK == ret)
//        {
//            if('A' == RX_USART_REG)
//            {
//                led_turn_on(&led1);
//                ret = EUSART_ASYNC_WriteStringBlocking("LED ON\r",7);
//            }
//            
//            else if('B' == RX_USART_REG)
//            {
//                led_turn_off(&led1);
//                ret = EUSART_ASYNC_WriteStringBlocking("LED OFF\r",8);
//            }
//         }
        
        
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = led_intialize(&led1);
    ret = led_intialize(&led2);
}


void EUSART_TxDefaultInterruptHandler(void)
{
    std_ReturnType ret = E_NOT_OK;
    
    
}

void EUSART_RxDefaultInterruptHandler(void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = EUSART_ASYNC_ReadByteNonBlocking(&RX_USART_REG);
    switch(RX_USART_REG)
    {
        case 'a' : 
            led_turn_on(&led1);
            ret = EUSART_ASYNC_WriteStringBlocking("LED ON\r",7);
            break;
        case 'b' :  
            led_turn_on(&led2);
            ret = EUSART_ASYNC_WriteStringBlocking("LED ON\r",7);
            break;
        case 'c' :  
            led_turn_off(&led1);
            ret = EUSART_ASYNC_WriteStringBlocking("LED OFF\r",8);
            break;
        case 'd' :  
            led_turn_off(&led2);
            ret = EUSART_ASYNC_WriteStringBlocking("LED OFF\r",8);
            break;
        default :
            led_turn_off(&led1);
            led_turn_off(&led2);
            ret = EUSART_ASYNC_WriteStringBlocking("LED OFF\r",8);
    }           
            
}

void EUSART_FramingErrorHandler (void)
{
    std_ReturnType ret = E_OK;
    uint8 Data_In_RCREG = 0;
    ret = EUSART_ASYNC_ReadByteNonBlocking(&Data_In_RCREG);
    
}
void EUSART_OverrunErrorHandler (void)
{
    std_ReturnType ret = E_OK;
    ret = EUSART_ASYNC_RX_Restart();
}

