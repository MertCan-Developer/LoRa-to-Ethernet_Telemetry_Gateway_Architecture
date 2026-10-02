################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include st7789_library/Src/subdir.mk
-include My_W5500_Library/Src/subdir.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include BMP180_Sensor_Library/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := LoRa-to-Ethernet Telemetry Gateway Architecture
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.elf \

MAP_FILES += \
LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.list \


# All Target
all: main-build

# Main-build Target
main-build: LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.elf secondary-outputs

# Tool invocations
LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.elf LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.map: $(OBJS) $(USER_OBJS) C:\Users\Mert\STM32CubeIDE\workspace_2.1.1\LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture\STM32F030C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "LoRa-to-Ethernet Telemetry Gateway Architecture.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\Mert\STM32CubeIDE\workspace_2.1.1\LoRa-to-Ethernet Telemetry Gateway Architecture\STM32F030C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="LoRa-to-Ethernet Telemetry Gateway Architecture.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LoRa-to-Ethernet Telemetry Gateway Architecture.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.elf LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.list LoRa-to-Ethernet\ Telemetry\ Gateway\ Architecture.map LoRa-to-Ethernet\\ Telemetry\\ Gateway\\ Architecture.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
