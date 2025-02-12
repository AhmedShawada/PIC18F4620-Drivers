/* 
 * File:   ecu_keypad.c
 * Author: ashaw
 *
 * Created on August 20, 2024, 4:05 PM
 */

#include "ecu_keypad.h"
//#define _XTAL_FREQ 4000000 

static const uint8 btn_values[ECU_KEYPAD_ROWS][ECU_KEYPAD_COLUMNS] = { 
                                                                        {'7','8','9','/'},
                                                                        {'4','5','6','*'},
                                                                        {'1','2','3','-'},
                                                                        {'C','0','=','+'}
                                                                     };

/**
 * @brief Initialize the keypad assigned pins
 * @param _keypad_obj pointer to the keypad configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
std_ReturnType keypad_intialize (const keypad_t *_keypad_obj)
{
    std_ReturnType ret = E_OK;
    uint8 rows_counter = ZERO_INIT;
    uint8 columns_counter = ZERO_INIT;
    
    if(NULL ==_keypad_obj)
    {
      ret = E_NOT_OK;  
    }
    
    else
    {
      for(rows_counter = ZERO_INIT;rows_counter<ECU_KEYPAD_ROWS;rows_counter++)
      {
          ret = gpio_pin_intialize(&(_keypad_obj->keypad_row_pins[rows_counter]));
      }
      
      for(columns_counter = ZERO_INIT;columns_counter<ECU_KEYPAD_COLUMNS;columns_counter++)
      {
          ret = gpio_pin_intialize(&(_keypad_obj->keypad_row_pins[columns_counter]));
      }
    }
    
    return ret;
}


/**
 * @brief Get the value of the button pressed by the user and performing the scanning algorithm
 * @param _keypad_obj pointer to the keypad configurations
 * @param value Value of the button pressed by the user
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action 
 */
std_ReturnType keypad_get_value (const keypad_t *_keypad_obj, uint8 *value)
{
    std_ReturnType ret = E_OK;
    uint8 l_rows_counter = ZERO_INIT;
    uint8 l_columns_counter = ZERO_INIT;
    uint8 counter = ZERO_INIT;
    uint8 logic_column = ZERO_INIT;
    
    if((NULL ==_keypad_obj)||(NULL == value))
    {
      ret = E_NOT_OK; 
      
    }
    
    else
    {
      for(l_rows_counter= ZERO_INIT; l_rows_counter<ECU_KEYPAD_ROWS; l_rows_counter++)
      {
        for(counter = ZERO_INIT;counter<ECU_KEYPAD_ROWS;counter++)
        {
          ret = gpio_pin_write_logic(&(_keypad_obj->keypad_row_pins[counter]),GPIO_LOW);
        }
        
        ret = gpio_pin_write_logic(&(_keypad_obj->keypad_row_pins[l_rows_counter]),GPIO_HIGH);
        //__delay_ms(10);
        
        for(l_columns_counter= ZERO_INIT; l_columns_counter<ECU_KEYPAD_ROWS; l_columns_counter++)
        {
          ret = gpio_pin_read_logic(&(_keypad_obj->keypad_columns_pins[l_columns_counter]),&logic_column);
          if(GPIO_HIGH == logic_column)
          {
            *value = btn_values[l_rows_counter][l_columns_counter];
          }
        }
      } 
        
    }
    
    return ret;
    
}