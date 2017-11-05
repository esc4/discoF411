#include "LSM303DLHC.h"
#include "i2c.h"

void ACC_Conf()
{
	uint8_t TxBuffer1[7] = {
			0x57,				/*Reg : CTRL_REG1_A (ODR 100Hz + EN all axes) */
			0x00,				/*Reg : CTRL_REG2_A (no filter) */
			0x40,				/*Reg : CTRL_REG3_A (I1_AOI) */
			0x28,				/*Reg : CTRL_REG4_A (continuous update, Little Endian, FS : +-8g (4mg/LSB), HR) */
			0x00,				/*Reg : CTRL_REG5_A (0x04 = D4D)*/
			0x40,				/*Reg : CTRL_REG6_A (INT1 on PAD2)*/
			0x00				/*Reg : REFERENCE_A (Reference value for interrupt generation) */
	};

	uint8_t TxBuffer2 = 0x0A;	/*Reg : INT1_CFG_A (OR detection, X|Y > TH) */

	uint8_t TxBuffer3[2] = {
			0x02,				/*Reg : INT1_THS_A (2LSB*62mg/LSB = 124mg) */
			0x0A				/*Reg : INT1_DURATION_A ((1/100Hz)*10 = 100ms*/
	};

	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)CTRL_REG1_A |0x80, (uint16_t)I2C_MEMADD_SIZE_8BIT, TxBuffer1, 7, 1000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}

/*CFG INT1 : OR x axe interrupt */
	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)INT1_CFG_A, (uint16_t)I2C_MEMADD_SIZE_8BIT, &TxBuffer2, 1, 1000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}

/*INT1 threshold & duration*/
	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)INT1_THS_A |0x80, (uint16_t)I2C_MEMADD_SIZE_8BIT, TxBuffer3, 2, 1000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
}

void MAG_Conf()
{
	uint8_t TxBufferMAGconf[3] = {
			0x90,				/* [Temp enable, ODR : 15Hz] [Range +/- 1.3 Gauss] [Continuous-conversion mode] */
			0x20,
			0x00
	};

	if((HAL_I2C_Mem_Write(&hi2c1, (uint16_t)MAG_ADD, (uint16_t)CRA_REG_M, (uint16_t)I2C_MEMADD_SIZE_8BIT, TxBufferMAGconf, 3, 5000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
}

void ACC_GetXYZ(int16_t* pX, int16_t* pY, int16_t* pZ)
{
	uint8_t RxBufferACC[6] = {0,0,0,0,0,0};
	if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)ACC_ADD, (uint16_t)OUT_X_L_A |0x80, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)RxBufferACC, (uint16_t)6, 1000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
	//12 bits data outputs
	*pX = (RxBufferACC[1]<<8) | RxBufferACC[0] ;
	*pX /= 16;
	*pY = (RxBufferACC[3]<<8) | RxBufferACC[2] ;
	*pY /= 16;
	*pZ = (RxBufferACC[5]<<8) | RxBufferACC[4] ;
	*pZ /= 16;
}

void ACC_LedMode()
{
	int16_t x,y,z = 0;
	ACC_GetXYZ(&x,&y,&z);
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
}

void MAG_GetXYZ(int16_t* pX, int16_t* pY, int16_t* pZ)
{
	uint8_t RxBufferMAG[6] = {0,0,0,0,0,0};
	if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)MAG_ADD, (uint16_t)OUT_X_H_M, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)RxBufferMAG, (uint16_t)6, 5000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
	// 16 bits data outputs ?? Seems to.
	*pX = (RxBufferMAG[0]<<8) | RxBufferMAG[1] ;	// Divide by 1100 to convert in Gauss
	*pZ = (RxBufferMAG[2]<<8) | RxBufferMAG[3] ;	// Divide by 980 to convert in Gauss
	*pY = (RxBufferMAG[4]<<8) | RxBufferMAG[5] ;	// Divide by 1100 to convert in Gauss
}

void MAG_GetTemp(int16_t* pT)
{
	uint8_t RxBufferTemp[2] = {0,0};
	if((HAL_I2C_Mem_Read(&hi2c1, (uint16_t)MAG_ADD, (uint16_t)TEMP_OUT_H_M, (uint16_t)I2C_MEMADD_SIZE_8BIT, (uint8_t *)RxBufferTemp, (uint16_t)2, 5000)) != HAL_OK)
	{
	  if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
	  {
		Error_Handler();
	  }
	}
	*pT = (RxBufferTemp[0]<<8) | RxBufferTemp[1] ;
	*pT /= 16;	 //12 Bits outputs
	*pT /= 8;	//8 LSB/°C (not calibrated)
}

void MAG_CompassMode()
{
	int16_t x,y,z = 0;
	MAG_GetXYZ(&x,&y,&z);

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
