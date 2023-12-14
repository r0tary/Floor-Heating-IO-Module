################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/hci_tl.c \
../Core/Inc/hci_tl_if.c \
../Core/Inc/shci_tl.c \
../Core/Inc/shci_tl_if.c \
../Core/Inc/tl_mbox.c \
../Core/Inc/tl_thread_hci.c \
../Core/Inc/tl_zigbee_hci.c 

OBJS += \
./Core/Inc/hci_tl.o \
./Core/Inc/hci_tl_if.o \
./Core/Inc/shci_tl.o \
./Core/Inc/shci_tl_if.o \
./Core/Inc/tl_mbox.o \
./Core/Inc/tl_thread_hci.o \
./Core/Inc/tl_zigbee_hci.o 

C_DEPS += \
./Core/Inc/hci_tl.d \
./Core/Inc/hci_tl_if.d \
./Core/Inc/shci_tl.d \
./Core/Inc/shci_tl_if.d \
./Core/Inc/tl_mbox.d \
./Core/Inc/tl_thread_hci.d \
./Core/Inc/tl_zigbee_hci.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/%.o Core/Inc/%.su Core/Inc/%.cyclo: ../Core/Inc/%.c Core/Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/MODBUS-LIB/Inc" -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/OLED_SSD1306/Inc" -I"../Middlewares/ST/STM32_WPAN" -I"../Middlewares/ST/STM32_WPAN/utilities" -I"../Middlewares/ST/STM32_WPAN/zigbee/platform" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/inc" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc

clean-Core-2f-Inc:
	-$(RM) ./Core/Inc/hci_tl.cyclo ./Core/Inc/hci_tl.d ./Core/Inc/hci_tl.o ./Core/Inc/hci_tl.su ./Core/Inc/hci_tl_if.cyclo ./Core/Inc/hci_tl_if.d ./Core/Inc/hci_tl_if.o ./Core/Inc/hci_tl_if.su ./Core/Inc/shci_tl.cyclo ./Core/Inc/shci_tl.d ./Core/Inc/shci_tl.o ./Core/Inc/shci_tl.su ./Core/Inc/shci_tl_if.cyclo ./Core/Inc/shci_tl_if.d ./Core/Inc/shci_tl_if.o ./Core/Inc/shci_tl_if.su ./Core/Inc/tl_mbox.cyclo ./Core/Inc/tl_mbox.d ./Core/Inc/tl_mbox.o ./Core/Inc/tl_mbox.su ./Core/Inc/tl_thread_hci.cyclo ./Core/Inc/tl_thread_hci.d ./Core/Inc/tl_thread_hci.o ./Core/Inc/tl_thread_hci.su ./Core/Inc/tl_zigbee_hci.cyclo ./Core/Inc/tl_zigbee_hci.d ./Core/Inc/tl_zigbee_hci.o ./Core/Inc/tl_zigbee_hci.su

.PHONY: clean-Core-2f-Inc

