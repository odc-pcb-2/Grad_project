#ifndef _HAL_LCD_CONFIG_H_
#define _HAL_LCD_CONFIG_H_

/* Options: EIGHT_BIT, FOUR_BIT */
#define LCD_MODE        FOUR_BIT

/* Options: LOW_NIBBLE, HIGH_NIBBLE */
#define NIBBLE_MODE     HIGH_NIBBLE

/* Options: GPIOA, GPIOB, GPIOC, GPIOD */
#define LCD_DATA_PORT   GPIOD//GPIOA

/* Options:  GPIOA, GPIOB, GPIOC, GPIOD  */
#define LCD_RS_PORT     GPIOD//GPIOB

/* Options: PIN0.........PIN7 */
#define LCD_RS_PIN      PIN0//PIN1

/* Options:  GPIOA, GPIOB, GPIOC, GPIOD  */
#define LCD_RW_PORT     GPIOD//GPIOB

/* Options: PIN0.........PIN7 */
#define LCD_RW_PIN      PIN1//PIN2

/* Options:  GPIOA, GPIOB, GPIOC, GPIOD  */
#define LCD_EN_PORT     GPIOD//GPIOB

/* Options: PIN0.........PIN7 */
#define LCD_EN_PIN      PIN2//PIN3

#endif