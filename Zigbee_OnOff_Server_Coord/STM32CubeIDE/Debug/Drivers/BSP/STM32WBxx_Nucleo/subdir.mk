################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Virtualiden/STM32Cube/Repository/STM32Cube_FW_WB_V1.17.2/Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.o: C:/Users/Virtualiden/STM32Cube/Repository/STM32Cube_FW_WB_V1.17.2/Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.c Drivers/BSP/STM32WBxx_Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32WBXX_NUCLEO -DZIGBEE_WB -DDEBUG -DSTM32WB55xx -c -I../../Core/Inc -I../../../../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/extras -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zgp -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/key -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/se -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/security -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/platform -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/core/inc -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32WBxx_Nucleo

clean-Drivers-2f-BSP-2f-STM32WBxx_Nucleo:
	-$(RM) ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.cyclo ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.d ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.o ./Drivers/BSP/STM32WBxx_Nucleo/stm32wbxx_nucleo.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32WBxx_Nucleo

