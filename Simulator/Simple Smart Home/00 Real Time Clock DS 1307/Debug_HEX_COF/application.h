/* 
 * File:   application.h
 * Author: ashaw
 *
 * Created on July 12, 2024, 11:48 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/*Section       :         Includes                */
#include "ECU_Layer/LED/ecu_led.h"
#include "ECU_Layer/Button/ecu_button.h"
#include "ECU_Layer/Relay/ecu_relay.h"
#include "ECU_Layer/DC_Motor/ecu_dc_motor.h"
#include "ECU_Layer/7-Segment/ecu_seven_seg.h"
#include "ECU_Layer/Keypad/ecu_keypad.h"
#include "ECU_Layer/Chr_LCD/ecu_chr_lcd.h"



/*Section       :         Data type Decleration                 */

/*Section       :         Macro Decleration                */


/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */
void application_intialize (void);
#endif	/* APPLICATION_H */

