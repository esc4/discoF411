/*
 * Dev.c
 *
 *  Created on: 29 oct. 2017
 *      Author: Quentin
 */

#include "Dev.h"


void DEV_LedInterlude1()
{
	for(int i=0; i<4; i++)
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	}
}
void DEV_LedInterlude2()
{
	for (int i=0; i<3; i++)
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	}
}

int16_t ConvertTwoComplement(uint16_t input)
{
	int16_t converted = 0;
	if (input > 2048)
	{
		converted = -((~input)+1);
	}
	else if(input == 2048)
	{
		converted = -2048;
	}
	else
	{
		converted = input;
	}

	return converted;
}

uint8_t CheckBit(uint8_t bitInt, uint8_t pos)
{
	return (bitInt & (1<<pos));
}
