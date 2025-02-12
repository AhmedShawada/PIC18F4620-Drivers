/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on August 31, 2024, 12:32 AM
 */


#include "application.h"


chr_lcd_4bits_t lcd_1 = {
    .lcd_rs.port = PORTD_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,

    .lcd_en.port = PORTD_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.logic = GPIO_LOW,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,

    .lcd_data[0].port = PORTD_INDEX,
    .lcd_data[0].pin = GPIO_PIN2,
    .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,

    .lcd_data[1].port = PORTD_INDEX,
    .lcd_data[1].pin = GPIO_PIN3,
    .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,

    .lcd_data[2].port = PORTD_INDEX,
    .lcd_data[2].pin = GPIO_PIN4,
    .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,

    .lcd_data[3].port = PORTD_INDEX,
    .lcd_data[3].pin = GPIO_PIN5,
    .lcd_data[3].logic = GPIO_LOW,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT 
};


keypad_t keypad = {
    .keypad_row_pins[0].port = PORTC_INDEX,
    .keypad_row_pins[0].pin = GPIO_PIN0,
    .keypad_row_pins[0].logic = GPIO_LOW,
    .keypad_row_pins[0].direction = GPIO_DIRECTION_OUTPUT,

    .keypad_row_pins[1].port = PORTC_INDEX,
    .keypad_row_pins[1].pin = GPIO_PIN1,
    .keypad_row_pins[1].logic = GPIO_LOW,
    .keypad_row_pins[1].direction = GPIO_DIRECTION_OUTPUT,

    .keypad_row_pins[2].port = PORTC_INDEX,
    .keypad_row_pins[2].pin = GPIO_PIN2,
    .keypad_row_pins[2].logic = GPIO_LOW,
    .keypad_row_pins[2].direction = GPIO_DIRECTION_OUTPUT,

    .keypad_row_pins[3].port = PORTC_INDEX,
    .keypad_row_pins[3].pin = GPIO_PIN3,
    .keypad_row_pins[3].logic = GPIO_LOW,
    .keypad_row_pins[3].direction = GPIO_DIRECTION_OUTPUT,

    .keypad_columns_pins[0].port = PORTC_INDEX,
    .keypad_columns_pins[0].pin = GPIO_PIN4,
    .keypad_columns_pins[0].logic = GPIO_LOW,
    .keypad_columns_pins[0].direction = GPIO_DIRECTION_INPUT,

    .keypad_columns_pins[1].port = PORTC_INDEX,
    .keypad_columns_pins[1].pin = GPIO_PIN5,
    .keypad_columns_pins[1].logic = GPIO_LOW,
    .keypad_columns_pins[1].direction = GPIO_DIRECTION_INPUT,

    .keypad_columns_pins[2].port = PORTC_INDEX,
    .keypad_columns_pins[2].pin = GPIO_PIN6,
    .keypad_columns_pins[2].logic = GPIO_LOW,
    .keypad_columns_pins[2].direction = GPIO_DIRECTION_INPUT,

    .keypad_columns_pins[3].port = PORTC_INDEX,
    .keypad_columns_pins[3].pin = GPIO_PIN7,
    .keypad_columns_pins[3].logic = GPIO_LOW,
    .keypad_columns_pins[3].direction = GPIO_DIRECTION_INPUT,
};

uint8 keypad_value = ZERO_INIT;
uint8 previous_keypad_value = ZERO_INIT;
uint8 operand1 = 0, operand2 = 0;
char operator = '\0';
uint8 result = 0;
uint8 result_txt[4];

void main() {
    std_ReturnType ret = E_NOT_OK;
    application_intialize();

    
    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 3, "CASIO CALCULATOR");
    __delay_ms(150);
    ret = lcd_4bits_send_command(&lcd_1, _LCD_CLEAR);

    while (1) {
        ret = keypad_get_value(&keypad, &keypad_value);

        if (keypad_value != 0 && keypad_value != previous_keypad_value)
        {
            previous_keypad_value = keypad_value;

            if (keypad_value >= '0' && keypad_value <= '9')
            {
                if (operator == '\0')
                {
                    operand1 = (operand1 * 10) + (keypad_value - '0');
                } 
                
                else
                {
                    operand2 = (operand2 * 10) + (keypad_value - '0');
                }
                ret = lcd_4bits_send_char_data(&lcd_1, keypad_value);
            } 
            
            else if ((keypad_value == '+') || (keypad_value == '-') || (keypad_value == '*') || (keypad_value == '/'))
            {
                operator = keypad_value;
                ret = lcd_4bits_send_char_data(&lcd_1, operator);
            } 
            
            else if (keypad_value == '=')
            {
                if (operator == '/' && operand2 == 0)
                {
                    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 1, "Error");
                } 
                
                else
                {
                    switch (operator) 
                    {
                        case '+': result = operand1 + operand2; break;
                        case '-': result = operand1 - operand2; break;
                        case '*': result = operand1 * operand2; break;
                        case '/': result = operand1 / operand2; break;
                        default: result = 0;
                    }
                    
                    convert_uint8_to_string(result, result_txt);
                    ret = lcd_4bits_send_command(&lcd_1, _LCD_CLEAR);
                    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 1, result_txt);
                    
                    operand1 = result;
                    operand2 = 0;
                    operator = '\0';
                }
            } 
            
            else if (keypad_value == 'C')
            {
                ret = lcd_4bits_send_command(&lcd_1, _LCD_CLEAR);
                operand1 = operand2 = 0;
                operator = '\0';
            }
                
        }
    }
}

void application_intialize(void) {
    std_ReturnType ret = E_NOT_OK;
    ret = lcd_4bits_intialize(&lcd_1);
    ret = keypad_intialize(&keypad);
}
