#ifndef __LED_H__
#define __LED_H__
/**********************
*       INCLUDE        
**********************/
#include "stm32f1xx_hal.h"
/**********************
*       DEFINE
**********************/
#define LED0_GPIO_PORT GPIOB
#define LED0_PIN GPIO_PIN_5
#define LED0(x) HAL_GPIO_WritePin(LED0_GPIO_PORT,LED0_PIN,x)

#define LED1_GPIO_PORT GPIOE
#define LED1_PIN GPIO_PIN_5
#define LED1(x) HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,x)
/**********************
*   EXTERN VARIABLE
**********************/

/**********************
*  FUNCTIONS DECLARE
**********************/
void LED_init(void);
#endif