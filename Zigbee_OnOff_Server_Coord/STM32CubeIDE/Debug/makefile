################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/subdir.mk
-include Middlewares/STM32_WPAN/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/STM32WBxx_Nucleo/subdir.mk
-include Application/User/Startup/subdir.mk
-include Application/User/STM32_WPAN/Target/subdir.mk
-include Application/User/STM32_WPAN/App/subdir.mk
-include Application/User/Core/subdir.mk
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


BUILD_ARTIFACT_NAME := Zigbee_OnOff_Server_Coord
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Zigbee_OnOff_Server_Coord.elf \

MAP_FILES += \
Zigbee_OnOff_Server_Coord.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Zigbee_OnOff_Server_Coord.list \

OBJCOPY_BIN += \
Zigbee_OnOff_Server_Coord.bin \


# All Target
all: main-build

# Main-build Target
main-build: Zigbee_OnOff_Server_Coord.elf secondary-outputs

# Tool invocations
Zigbee_OnOff_Server_Coord.elf Zigbee_OnOff_Server_Coord.map: $(OBJS) $(USER_OBJS) C:\Users\Virtualiden\STM32Cube\Repository\STM32Cube_FW_WB_V1.17.2\Projects\P-NUCLEO-WB55.Nucleo\Applications\Zigbee\Zigbee_OnOff_Server_Coord\STM32CubeIDE\STM32WB55RGVX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Zigbee_OnOff_Server_Coord.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Virtualiden\STM32Cube\Repository\STM32Cube_FW_WB_V1.17.2\Projects\P-NUCLEO-WB55.Nucleo\Applications\Zigbee\Zigbee_OnOff_Server_Coord\STM32CubeIDE\STM32WB55RGVX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Zigbee_OnOff_Server_Coord.map" -Wl,--gc-sections -static -L../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/lib --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Zigbee_OnOff_Server_Coord.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Zigbee_OnOff_Server_Coord.list"
	@echo 'Finished building: $@'
	@echo ' '

Zigbee_OnOff_Server_Coord.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Zigbee_OnOff_Server_Coord.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Zigbee_OnOff_Server_Coord.bin Zigbee_OnOff_Server_Coord.elf Zigbee_OnOff_Server_Coord.list Zigbee_OnOff_Server_Coord.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
