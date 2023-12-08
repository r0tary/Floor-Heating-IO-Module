################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.c \
../Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.c \
../Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.c \
../Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.c 

OBJS += \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.o \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.o \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.o \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.o 

C_DEPS += \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.d \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.d \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.d \
./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/%.o Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/%.su Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/%.cyclo: ../Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/%.c Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32WBXX_NUCLEO -DZIGBEE_WB -DDEBUG -DSTM32WB55xx -c -I../../Core/Inc -I../../../../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -I../../STM32_WPAN/App -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/extras -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zgp -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/key -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/se -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/security -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/platform -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/core/inc -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-STM32_WPAN-2f-zigbee-2f-stack-2f-zcl-2f-st_custom

clean-Middlewares-2f-STM32_WPAN-2f-zigbee-2f-stack-2f-zcl-2f-st_custom:
	-$(RM) ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.cyclo ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.d ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.o ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_client.su ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.cyclo ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.d ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.o ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_ls_server.su ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.cyclo ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.d ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.o ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_client.su ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.cyclo ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.d ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.o ./Middlewares/STM32_WPAN/zigbee/stack/zcl/st_custom/zcl_custom_matrix_server.su

.PHONY: clean-Middlewares-2f-STM32_WPAN-2f-zigbee-2f-stack-2f-zcl-2f-st_custom

