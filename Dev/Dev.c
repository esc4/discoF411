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

void DEV_I2CReadRegister(uint16_t add, uint16_t registeradd, uint8_t * pBuffer)
{
	if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)add, (uint16_t)registeradd, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, (uint16_t)6, 10000)) != HAL_OK)
		{
		  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		  {
			Error_Handler();
		  }
		}
}
