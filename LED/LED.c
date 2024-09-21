/**********************
*       INCLUDE        
**********************/
#include "LED.h"
/**********************
*       DEFINE
**********************/

/**********************
*   GLOBAL VARIABLE
**********************/

/**********************
*     FUNCTIONS
**********************/
/** 
* @brief
* @param
* @return
*/
void LED_init(void)
{
    GPIO_InitTypeDef gpio_initstructure;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    gpio_initstructure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initstructure.Pin = GPIO_PIN_5;
    gpio_initstructure.Pull = GPIO_PULLUP;
    gpio_initstructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpio_initstructure);
    HAL_GPIO_Init(GPIOE,&gpio_initstructure);

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
}