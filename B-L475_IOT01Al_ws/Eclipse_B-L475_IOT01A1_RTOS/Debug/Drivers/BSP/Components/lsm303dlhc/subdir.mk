################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/lsm303dlhc/lsm303dlhc.c 

OBJS += \
./Drivers/BSP/Components/lsm303dlhc/lsm303dlhc.o 

C_DEPS += \
./Drivers/BSP/Components/lsm303dlhc/lsm303dlhc.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/lsm303dlhc/%.o: ../Drivers/BSP/Components/lsm303dlhc/%.c Drivers/BSP/Components/lsm303dlhc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"E:/Documents/UMD/Spring2022/ENEE459V/Homework/Felix_ENEE459v_HW11/Problem2/B-L475_IOT01Al_ws/Eclipse_B-L475_IOT01A1_RTOS/Drivers/BSP/B-L475E-IOT01" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-lsm303dlhc

clean-Drivers-2f-BSP-2f-Components-2f-lsm303dlhc:
	-$(RM) ./Drivers/BSP/Components/lsm303dlhc/lsm303dlhc.d ./Drivers/BSP/Components/lsm303dlhc/lsm303dlhc.o

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-lsm303dlhc

