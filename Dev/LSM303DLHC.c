#include "LSM303DLHC.h"
#include "i2c.h"

void ACC_Conf()
{
	uint8_t TxBufferCtrlRegEnAll = 0x33; // Enable 1 Hz + Enable all axes
	uint8_t TxBufferCTRL_REG4_A = 0xA8;	// output registers not updated until MSB and LSB have been read

	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)CTRL_REG1_A, (uint16_t)I2C_MEMADD_SIZE_8BIT, &TxBufferCtrlRegEnAll, 1, 10000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}

	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)CTRL_REG4_A, (uint16_t)I2C_MEMADD_SIZE_8BIT, &TxBufferCTRL_REG4_A, 1, 10000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
}

void MAG_Conf()
{

}
void ACC_GetXYZ(uint8_t* pBuffer, int16_t* pX, int16_t* pY, int16_t* pZ)
{
  if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)OUT_X_L_A |0x80, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, (uint16_t)6, 10000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
	*pX = (pBuffer[1]<<8) | pBuffer[0] ;
	*pX /= 16;
	*pY = (pBuffer[3]<<8) | pBuffer[2] ;
	*pY /= 16;
	*pZ = (pBuffer[5]<<8) | pBuffer[4] ;
	*pZ /= 16;
}

void ACC_LedMode(int16_t x, int16_t y, int16_t z)
{
//X axe
	if(x < -20)
	{
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	}
	else if(x > 20)
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	}
	else
	{
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	}
//Y axe
	if(y < -20)
	{
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	}
	else if(y > 20)
	{
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	}
	else
	{
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	}
//Z axe


}

void MAG_GetXYZ(uint8_t* pBuffer, int16_t* pX, int16_t* pY, int16_t* pZ)
{

}
