/* 
 * File:   ecu_relay.h
 * Author: ashaw
 *
 * Created on July 27, 2024, 9:39 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

/*Section       :         Includes                */
#include "ecu_relay_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/*Section       :         Data type Decleration                 */
typedef struct{
    uint8 relay_port   : 4;
    uint8 relay_pin    : 3;
    uint8 relay_status : 1;
}relay_t;


/*Section       :         Macro Decleration                */
#define RELAY_ON_STATUS 0x01U
#define RELAY_OFF_STATUS 0x00U

/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
std_ReturnType relay_intialize (const relay_t *_relay);
std_ReturnType relay_turn_on (const relay_t *_relay);
std_ReturnType relay_turn_off (const relay_t *_relay);

#endif	/* ECU_RELAY_H */

