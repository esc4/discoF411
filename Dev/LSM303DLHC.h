#ifndef __LSM303_H

#define __LSM303_H
#include "stm32f4xx_hal.h"

#define ACC_ADD (0x19<<1)
#define CTRL_REG1_A (0x20)
#define CTRL_REG4_A (0x23)
#define STATUS_REG_A (0x27)
#define OUT_X_L_A (0x28)
#define OUT_X_H_A (0x29)

#define MAG_ADD (0x1E<<1)
#define CRA_REG_M (0x00)
#define CRB_REG_M (0x01)
#define MR_REG_M (0x02)
#define OUT_X_H_M (0x03)
#define TEMP_OUT_H_M (0x31)

float atan2f(float, float);

void ACC_Conf();
void ACC_GetXYZ(uint8_t*, int16_t*, int16_t*, int16_t*);
void ACC_LedMode(int16_t, int16_t, int16_t);

void MAG_Conf();
void MAG_GetXYZ(uint8_t*, int16_t*, int16_t*, int16_t*);
void MAG_GetTemp(uint8_t*, int16_t*);
void MAG_CompassMode(int16_t, int16_t);

#endif



