/* 
 * File:   RealTimeClockDS1307.h
 * Author: ahmed-abdallah
 *
 * Created on November 1, 2024, 11:28 PM
 */

#ifndef REALTIMECLOCKDS1307_H
#define	REALTIMECLOCKDS1307_H

/*Section       :         Includes                */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/I2C/hal_i2c.h"
#include "../../MCAL_Layer/USART/hal_usart.h"
#include "../../ECU_Layer/Chr_LCD/ecu_chr_lcd.h"

/*Section       :         Data type Decleration                 */

typedef struct
{
    uint8 Seconds;
    uint8 Minutes;
    uint8 Hours;
    uint8 Days;
    uint8 Months;
    uint8 Years;
    
}RealTimeClock_T;

/*Section       :         Macro Decleration                */


/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */

RealTimeClock_T GET_DATA_DS1307(void);
void Print_RealTimeClockDS1307(void);
void LCD_RealTimeClockDS1307(void);

#endif	/* REALTIMECLOCKDS1307_H */

