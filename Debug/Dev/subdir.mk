################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dev/Dev.c \
../Dev/LSM303DLHC.c 

OBJS += \
./Dev/Dev.o \
./Dev/LSM303DLHC.o 

C_DEPS += \
./Dev/Dev.d \
./Dev/LSM303DLHC.d 


# Each subdirectory must supply rules for building sources it contributes
Dev/%.o: ../Dev/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F411xE -I"D:/Embedded_workspace/SW4STM32_workspace/F411_pr2/Inc" -I"D:/Embedded_workspace/SW4STM32_workspace/F411_pr2/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/Embedded_workspace/SW4STM32_workspace/F411_pr2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/Embedded_workspace/SW4STM32_workspace/F411_pr2/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/Embedded_workspace/SW4STM32_workspace/F411_pr2/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


