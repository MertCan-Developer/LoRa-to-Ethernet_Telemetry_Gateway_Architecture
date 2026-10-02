################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BMP180_Sensor_Library/Src/BMP180.c 

OBJS += \
./BMP180_Sensor_Library/Src/BMP180.o 

C_DEPS += \
./BMP180_Sensor_Library/Src/BMP180.d 


# Each subdirectory must supply rules for building sources it contributes
BMP180_Sensor_Library/Src/%.o BMP180_Sensor_Library/Src/%.su BMP180_Sensor_Library/Src/%.cyclo: ../BMP180_Sensor_Library/Src/%.c BMP180_Sensor_Library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/Ethernet" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/BMP180_Sensor_Library/Inc" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/st7789_library/Inc" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/My_W5500_Library/Inc" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/Ethernet/W5500" -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-BMP180_Sensor_Library-2f-Src

clean-BMP180_Sensor_Library-2f-Src:
	-$(RM) ./BMP180_Sensor_Library/Src/BMP180.cyclo ./BMP180_Sensor_Library/Src/BMP180.d ./BMP180_Sensor_Library/Src/BMP180.o ./BMP180_Sensor_Library/Src/BMP180.su

.PHONY: clean-BMP180_Sensor_Library-2f-Src

