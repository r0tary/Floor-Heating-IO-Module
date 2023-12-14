################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/hci_tl.c \
../Middlewares/ST/STM32_WPAN/hci_tl_if.c \
../Middlewares/ST/STM32_WPAN/shci_tl.c \
../Middlewares/ST/STM32_WPAN/shci_tl_if.c \
../Middlewares/ST/STM32_WPAN/tl_mbox.c \
../Middlewares/ST/STM32_WPAN/tl_thread_hci.c \
../Middlewares/ST/STM32_WPAN/tl_zigbee_hci.c 

OBJS += \
./Middlewares/ST/STM32_WPAN/hci_tl.o \
./Middlewares/ST/STM32_WPAN/hci_tl_if.o \
./Middlewares/ST/STM32_WPAN/shci_tl.o \
./Middlewares/ST/STM32_WPAN/shci_tl_if.o \
./Middlewares/ST/STM32_WPAN/tl_mbox.o \
./Middlewares/ST/STM32_WPAN/tl_thread_hci.o \
./Middlewares/ST/STM32_WPAN/tl_zigbee_hci.o 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/hci_tl.d \
./Middlewares/ST/STM32_WPAN/hci_tl_if.d \
./Middlewares/ST/STM32_WPAN/shci_tl.d \
./Middlewares/ST/STM32_WPAN/shci_tl_if.d \
./Middlewares/ST/STM32_WPAN/tl_mbox.d \
./Middlewares/ST/STM32_WPAN/tl_thread_hci.d \
./Middlewares/ST/STM32_WPAN/tl_zigbee_hci.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_WPAN/%.o Middlewares/ST/STM32_WPAN/%.su Middlewares/ST/STM32_WPAN/%.cyclo: ../Middlewares/ST/STM32_WPAN/%.c Middlewares/ST/STM32_WPAN/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/MODBUS-LIB/Inc" -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/OLED_SSD1306/Inc" -I"../Middlewares/ST/STM32_WPAN" -I"../Middlewares/ST/STM32_WPAN/utilities" -I"../Middlewares/ST/STM32_WPAN/zigbee/platform" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/inc" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_WPAN

clean-Middlewares-2f-ST-2f-STM32_WPAN:
	-$(RM) ./Middlewares/ST/STM32_WPAN/hci_tl.cyclo ./Middlewares/ST/STM32_WPAN/hci_tl.d ./Middlewares/ST/STM32_WPAN/hci_tl.o ./Middlewares/ST/STM32_WPAN/hci_tl.su ./Middlewares/ST/STM32_WPAN/hci_tl_if.cyclo ./Middlewares/ST/STM32_WPAN/hci_tl_if.d ./Middlewares/ST/STM32_WPAN/hci_tl_if.o ./Middlewares/ST/STM32_WPAN/hci_tl_if.su ./Middlewares/ST/STM32_WPAN/shci_tl.cyclo ./Middlewares/ST/STM32_WPAN/shci_tl.d ./Middlewares/ST/STM32_WPAN/shci_tl.o ./Middlewares/ST/STM32_WPAN/shci_tl.su ./Middlewares/ST/STM32_WPAN/shci_tl_if.cyclo ./Middlewares/ST/STM32_WPAN/shci_tl_if.d ./Middlewares/ST/STM32_WPAN/shci_tl_if.o ./Middlewares/ST/STM32_WPAN/shci_tl_if.su ./Middlewares/ST/STM32_WPAN/tl_mbox.cyclo ./Middlewares/ST/STM32_WPAN/tl_mbox.d ./Middlewares/ST/STM32_WPAN/tl_mbox.o ./Middlewares/ST/STM32_WPAN/tl_mbox.su ./Middlewares/ST/STM32_WPAN/tl_thread_hci.cyclo ./Middlewares/ST/STM32_WPAN/tl_thread_hci.d ./Middlewares/ST/STM32_WPAN/tl_thread_hci.o ./Middlewares/ST/STM32_WPAN/tl_thread_hci.su ./Middlewares/ST/STM32_WPAN/tl_zigbee_hci.cyclo ./Middlewares/ST/STM32_WPAN/tl_zigbee_hci.d ./Middlewares/ST/STM32_WPAN/tl_zigbee_hci.o ./Middlewares/ST/STM32_WPAN/tl_zigbee_hci.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_WPAN

