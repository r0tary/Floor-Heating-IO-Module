################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/PID0.c \
../Core/Src/app_entry.c \
../Core/Src/app_freertos.c \
../Core/Src/freertos_port.c \
../Core/Src/hw_timerserver.c \
../Core/Src/hw_uart.c \
../Core/Src/main.c \
../Core/Src/stm32_lpm_if.c \
../Core/Src/stm32wbxx_hal_msp.c \
../Core/Src/stm32wbxx_hal_timebase_tim.c \
../Core/Src/stm32wbxx_it.c \
../Core/Src/stm_logging.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32wbxx.c 

OBJS += \
./Core/Src/PID0.o \
./Core/Src/app_entry.o \
./Core/Src/app_freertos.o \
./Core/Src/freertos_port.o \
./Core/Src/hw_timerserver.o \
./Core/Src/hw_uart.o \
./Core/Src/main.o \
./Core/Src/stm32_lpm_if.o \
./Core/Src/stm32wbxx_hal_msp.o \
./Core/Src/stm32wbxx_hal_timebase_tim.o \
./Core/Src/stm32wbxx_it.o \
./Core/Src/stm_logging.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32wbxx.o 

C_DEPS += \
./Core/Src/PID0.d \
./Core/Src/app_entry.d \
./Core/Src/app_freertos.d \
./Core/Src/freertos_port.d \
./Core/Src/hw_timerserver.d \
./Core/Src/hw_uart.d \
./Core/Src/main.d \
./Core/Src/stm32_lpm_if.d \
./Core/Src/stm32wbxx_hal_msp.d \
./Core/Src/stm32wbxx_hal_timebase_tim.d \
./Core/Src/stm32wbxx_it.d \
./Core/Src/stm_logging.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32wbxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/MODBUS-LIB/Inc" -I"C:/Users/Virtualiden/Documents/GitHub/Floor-Heating-IO-Module/IO_MODULE_proj/Middlewares/Third_Party/OLED_SSD1306/Inc" -I"../Middlewares/ST/STM32_WPAN" -I"../Middlewares/ST/STM32_WPAN/utilities" -I"../Middlewares/ST/STM32_WPAN/zigbee/platform" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl" -I"../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/inc" -I"../Middlewares/ST/STM32_WPAN/zigbee/core/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/PID0.cyclo ./Core/Src/PID0.d ./Core/Src/PID0.o ./Core/Src/PID0.su ./Core/Src/app_entry.cyclo ./Core/Src/app_entry.d ./Core/Src/app_entry.o ./Core/Src/app_entry.su ./Core/Src/app_freertos.cyclo ./Core/Src/app_freertos.d ./Core/Src/app_freertos.o ./Core/Src/app_freertos.su ./Core/Src/freertos_port.cyclo ./Core/Src/freertos_port.d ./Core/Src/freertos_port.o ./Core/Src/freertos_port.su ./Core/Src/hw_timerserver.cyclo ./Core/Src/hw_timerserver.d ./Core/Src/hw_timerserver.o ./Core/Src/hw_timerserver.su ./Core/Src/hw_uart.cyclo ./Core/Src/hw_uart.d ./Core/Src/hw_uart.o ./Core/Src/hw_uart.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32_lpm_if.cyclo ./Core/Src/stm32_lpm_if.d ./Core/Src/stm32_lpm_if.o ./Core/Src/stm32_lpm_if.su ./Core/Src/stm32wbxx_hal_msp.cyclo ./Core/Src/stm32wbxx_hal_msp.d ./Core/Src/stm32wbxx_hal_msp.o ./Core/Src/stm32wbxx_hal_msp.su ./Core/Src/stm32wbxx_hal_timebase_tim.cyclo ./Core/Src/stm32wbxx_hal_timebase_tim.d ./Core/Src/stm32wbxx_hal_timebase_tim.o ./Core/Src/stm32wbxx_hal_timebase_tim.su ./Core/Src/stm32wbxx_it.cyclo ./Core/Src/stm32wbxx_it.d ./Core/Src/stm32wbxx_it.o ./Core/Src/stm32wbxx_it.su ./Core/Src/stm_logging.cyclo ./Core/Src/stm_logging.d ./Core/Src/stm_logging.o ./Core/Src/stm_logging.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32wbxx.cyclo ./Core/Src/system_stm32wbxx.d ./Core/Src/system_stm32wbxx.o ./Core/Src/system_stm32wbxx.su

.PHONY: clean-Core-2f-Src

