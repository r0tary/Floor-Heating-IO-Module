################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/STM32_WPAN/zigbee/platform/ee.c \
../Middlewares/STM32_WPAN/zigbee/platform/hw_flash.c 

OBJS += \
./Middlewares/STM32_WPAN/zigbee/platform/ee.o \
./Middlewares/STM32_WPAN/zigbee/platform/hw_flash.o 

C_DEPS += \
./Middlewares/STM32_WPAN/zigbee/platform/ee.d \
./Middlewares/STM32_WPAN/zigbee/platform/hw_flash.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_WPAN/zigbee/platform/%.o Middlewares/STM32_WPAN/zigbee/platform/%.su Middlewares/STM32_WPAN/zigbee/platform/%.cyclo: ../Middlewares/STM32_WPAN/zigbee/platform/%.c Middlewares/STM32_WPAN/zigbee/platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32WBXX_NUCLEO -DZIGBEE_WB -DDEBUG -DSTM32WB55xx -c -I../../Core/Inc -I../../../../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/extras -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zgp -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/key -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/se -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/security -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/platform -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/core/inc -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-STM32_WPAN-2f-zigbee-2f-platform

clean-Middlewares-2f-STM32_WPAN-2f-zigbee-2f-platform:
	-$(RM) ./Middlewares/STM32_WPAN/zigbee/platform/ee.cyclo ./Middlewares/STM32_WPAN/zigbee/platform/ee.d ./Middlewares/STM32_WPAN/zigbee/platform/ee.o ./Middlewares/STM32_WPAN/zigbee/platform/ee.su ./Middlewares/STM32_WPAN/zigbee/platform/hw_flash.cyclo ./Middlewares/STM32_WPAN/zigbee/platform/hw_flash.d ./Middlewares/STM32_WPAN/zigbee/platform/hw_flash.o ./Middlewares/STM32_WPAN/zigbee/platform/hw_flash.su

.PHONY: clean-Middlewares-2f-STM32_WPAN-2f-zigbee-2f-platform

