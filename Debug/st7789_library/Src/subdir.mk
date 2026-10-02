################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../st7789_library/Src/st7789.c 

OBJS += \
./st7789_library/Src/st7789.o 

C_DEPS += \
./st7789_library/Src/st7789.d 


# Each subdirectory must supply rules for building sources it contributes
st7789_library/Src/%.o st7789_library/Src/%.su st7789_library/Src/%.cyclo: ../st7789_library/Src/%.c st7789_library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/Ethernet" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/BMP180_Sensor_Library/Inc" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/st7789_library/Inc" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/My_W5500_Library/Inc" -I"C:/Users/Mert/STM32CubeIDE/workspace_2.1.1/LoRa-to-Ethernet Telemetry Gateway Architecture/Ethernet/W5500" -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-st7789_library-2f-Src

clean-st7789_library-2f-Src:
	-$(RM) ./st7789_library/Src/st7789.cyclo ./st7789_library/Src/st7789.d ./st7789_library/Src/st7789.o ./st7789_library/Src/st7789.su

.PHONY: clean-st7789_library-2f-Src

