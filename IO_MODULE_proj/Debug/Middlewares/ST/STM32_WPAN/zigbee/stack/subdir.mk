################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.c \
../Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.c 

OBJS += \
./Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.o \
./Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.o 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.d \
./Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_WPAN/zigbee/stack/%.o Middlewares/ST/STM32_WPAN/zigbee/stack/%.su Middlewares/ST/STM32_WPAN/zigbee/stack/%.cyclo: ../Middlewares/ST/STM32_WPAN/zigbee/stack/%.c Middlewares/ST/STM32_WPAN/zigbee/stack/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/MODBUS-LIB/Inc" -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/OLED_SSD1306/Inc" -I"../Middlewares/ST/STM32_WPAN" -I"../Middlewares/ST/STM32_WPAN/utilities" -I"../Middlewares/ST/STM32_WPAN/zigbee/platform" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/inc" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-zigbee-2f-stack

clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-zigbee-2f-stack:
	-$(RM) ./Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.cyclo ./Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.d ./Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.o ./Middlewares/ST/STM32_WPAN/zigbee/stack/pletoh.su ./Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.cyclo ./Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.d ./Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.o ./Middlewares/ST/STM32_WPAN/zigbee/stack/zigbee.string.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_WPAN-2f-zigbee-2f-stack

