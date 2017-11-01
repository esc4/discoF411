#include "LSM303DLHC.h"
#include "i2c.h"

void ACC_Conf()
{
	uint8_t TxBufferCtrlRegEnAll = 0x37; // Enable 1 Hz + Enable all axes
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
	uint8_t TxBufferMAGconf[3] = {0x90, 0x20, 0x00};	/* [Temp enable, ODR : 15Hz] [Range +/- 1.3 Gauss] [Continuous-conversion mode] */

	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)MAG_ADD, (uint16_t)CRA_REG_M, (uint16_t)I2C_MEMADD_SIZE_8BIT, TxBufferMAGconf, 3, 10000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
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
	  //12 bits data outputs
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
  if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)MAG_ADD, (uint16_t)OUT_X_H_M, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, (uint16_t)6, 10000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
  // 16 bits data outputs ?? Seems to.
	*pX = (pBuffer[0]<<8) | pBuffer[1] ;	// Divide by 1100 to convert in Gauss
	*pZ = (pBuffer[2]<<8) | pBuffer[3] ;	// Divide by 980 to convert in Gauss
	*pY = (pBuffer[4]<<8) | pBuffer[5] ;	// Divide by 1100 to convert in Gauss
}

void MAG_GetTemp(uint8_t* pBuffer, int16_t* pT)
{
  if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)MAG_ADD, (uint16_t)TEMP_OUT_H_M, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, (uint16_t)2, 10000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
  	*pT = (pBuffer[0]<<8) | pBuffer[1] ;
	*pT /= 16;	 //12 Bits outputs
	*pT /= 8;	//8 LSB/°C (not calibrated)
}

void MAG_CompassMode(int16_t x, int16_t y)
{
	float magDeclination = 1.483;	//Magnetic declination east/west depends on actual position
	float angle = atan2f((float)y, (float)x)*180/3.1415;
	angle += magDeclination;
	uint8_t accu = 10;	//Window detection +/- accu (°)

	if(angle < accu && angle > -accu)
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	}
	else if (angle < -(90-accu) && angle > -(90+accu) )
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	}
	else if(angle < -(180-accu) && angle > -(180+accu) )
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_SET);
	}
	else if (angle > (90-accu) && angle < (90+accu) )
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	}
	else
	{
	HAL_GPIO_WritePin(GPIOD,LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,LD6_Pin, GPIO_PIN_RESET);
	}
}
