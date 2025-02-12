/* 
 * File:   ecu_dc_motor.h
 * Author: ashaw
 *
 * Created on July 28, 2024, 10:44 PM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H

/*Section       :         Includes                */
#include "ecu_dc_motor_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/*Section       :         Data type Decleration                 */


typedef struct{
    pin_config_t dc_motor_pin[2];
}dc_motor_t;


/*Section       :         Macro Decleration                */
#define DC_MOTOR_ON_STATUS 0x01U
#define DC_MOTOR_OFF_STATUS 0x00U

/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
std_ReturnType dc_motor_intialize (const dc_motor_t *_dc_motor);
std_ReturnType dc_motor_move_right (const dc_motor_t *_dc_motor);
std_ReturnType dc_motor_move_left (const dc_motor_t *_dc_motor);
std_ReturnType dc_motor_stop (const dc_motor_t *_dc_motor);
#endif	/* ECU_DC_MOTOR_H */

