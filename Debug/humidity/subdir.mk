################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../humidity/hts221.c 

OBJS += \
./humidity/hts221.o 

C_DEPS += \
./humidity/hts221.d 


# Each subdirectory must supply rules for building sources it contributes
humidity/hts221.o: ../humidity/hts221.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F303x8 -DDEBUG -DUSE_FULL_LL_DRIVER -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../accelerometer -I../barometer -I../compass -I../humidity -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"humidity/hts221.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

