/* 
 * File:   UltraSonic.h
 * Author: ahmed-abdallah
 *
 * Created on November 7, 2024, 5:43 PM
 */

#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

/*Section       :         Includes                */

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/Timer1/hal_timer1.h"

/*Section       :         Data type Decleration                 */
typedef struct 
{
    pin_config_t Trigger_Pin;
    pin_config_t Echo_Pin;
    
}ultrasonic_t;

timer1_t timer1_timer_obj;

/*Section       :         Macro Decleration                */


/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
std_ReturnType Ultrasonic_Init(const ultrasonic_t *_config);
std_ReturnType Ultrasonic_Calculate_Distance(const ultrasonic_t *_config, uint16 *Distance);


#endif	/* ULTRASONIC_H */

