################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Ethernet/DHCP/dhcp.c 

OBJS += \
./Ethernet/DHCP/dhcp.o 

C_DEPS += \
./Ethernet/DHCP/dhcp.d 


# Each subdirectory must supply rules for building sources it contributes
Ethernet/DHCP/%.o Ethernet/DHCP/%.su Ethernet/DHCP/%.cyclo: ../Ethernet/DHCP/%.c Ethernet/DHCP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I"C:/Users/Mert/Documents/STM32_CubeMx_Files/stm32f030c8t6_ethernet/Ethernet" -I"C:/Users/Mert/Documents/STM32_CubeMx_Files/stm32f030c8t6_ethernet/st7789_library/Inc" -I"C:/Users/Mert/Documents/STM32_CubeMx_Files/stm32f030c8t6_ethernet/My_W5500_Library/Inc" -I"C:/Users/Mert/Documents/STM32_CubeMx_Files/stm32f030c8t6_ethernet/Ethernet/W5500" -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Ethernet-2f-DHCP

clean-Ethernet-2f-DHCP:
	-$(RM) ./Ethernet/DHCP/dhcp.cyclo ./Ethernet/DHCP/dhcp.d ./Ethernet/DHCP/dhcp.o ./Ethernet/DHCP/dhcp.su

.PHONY: clean-Ethernet-2f-DHCP

