/*
 * Dev.h
 *
 *  Created on: 29 oct. 2017
 *      Author: Quentin
 */

#ifndef DEV_H_
#define DEV_H_
#include "stm32f4xx_hal.h"

void DEV_LedInterlude1();
void DEV_LedInterlude2();

int16_t ConvertTwoComplement(uint16_t);
uint8_t CheckBit(uint8_t, uint8_t);


#endif /* DEV_H_ */


