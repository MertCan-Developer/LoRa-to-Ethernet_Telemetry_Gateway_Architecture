################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../W5500/socket.c \
../W5500/w5500.c \
../W5500/wizchip_conf.c 

OBJS += \
./W5500/socket.o \
./W5500/w5500.o \
./W5500/wizchip_conf.o 

C_DEPS += \
./W5500/socket.d \
./W5500/w5500.d \
./W5500/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
W5500/%.o W5500/%.su W5500/%.cyclo: ../W5500/%.c W5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-W5500

clean-W5500:
	-$(RM) ./W5500/socket.cyclo ./W5500/socket.d ./W5500/socket.o ./W5500/socket.su ./W5500/w5500.cyclo ./W5500/w5500.d ./W5500/w5500.o ./W5500/w5500.su ./W5500/wizchip_conf.cyclo ./W5500/wizchip_conf.d ./W5500/wizchip_conf.o ./W5500/wizchip_conf.su

.PHONY: clean-W5500

