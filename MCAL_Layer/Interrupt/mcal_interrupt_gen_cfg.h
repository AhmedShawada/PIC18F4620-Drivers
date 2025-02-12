/* 
 * File:   mcal_interrupt_gen_cfg.h
 * Author: ahmed-abdallah
 *
 * Created on September 12, 2024, 10:27 PM
 */

#ifndef MCAL_INTERRUPT_GEN_CFG_H
#define	MCAL_INTERRUPT_GEN_CFG_H



/*Section       :         Includes                */

#include "../pic18_chip_select.h"
#include "../mcal_std_types.h"
#include "mcal_interrupt_gen_cfg.h"

/*Section       :         Data type Decleration                 */

/*Section       :         Macro Decleration                */

#define INTERRUPT_FEATURE_ENABLE 1U

/* This macro will enable priority levels on interrupts. */

//#define INTERRUPT_PRIORITY_LEVELS_ENABLE              INTERRUPT_FEATURE_ENABLE

/*=============== INTx - RBx INTERRUPT FEATURE ENABLE ===============*/

#define EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE        INTERRUPT_FEATURE_ENABLE
#define EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE    INTERRUPT_FEATURE_ENABLE

/*=============== ADC INTERRUPT FEATURE ENABLE ===============*/

#define ADC_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE

/*=============== TIMERs INTERRUPT FEATURE ENABLE ===============*/

#define TIMER0_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE
#define TIMER1_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE
#define TIMER2_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE
#define TIMER3_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE

/*=============== CCP INTERRUPT FEATURE ENABLE ===============*/

#define CCP1_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE
#define CCP2_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE

/*=============== EUSART INTERRUPT FEATURE ENABLE ===============*/

#define EUSART_TX_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE
#define EUSART_RX_INTERRUPT_FEATURE_ENABLE                  INTERRUPT_FEATURE_ENABLE

/*=============== MSSP I2C INTERRUPT FEATURE ENABLE ===============*/

#define MSSP_I2C_INTERRUPT_FEATURE_ENABLE                   INTERRUPT_FEATURE_ENABLE
#define MSSP_I2C_BUS_COL_INTERRUPT_FEATURE_ENABLE           INTERRUPT_FEATURE_ENABLE
#define MSSP_I2C_Overflow_INTERRUPT_FEATURE_ENABLE          INTERRUPT_FEATURE_ENABLE 


/*Section       :         Macro Function Decleration                */

/*Section       :         Function Decleration                */

#endif	/* MCAL_INTERRUPT_GEN_CFG_H */

