################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.c \
../Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.c \
../Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.c 

OBJS += \
./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.o \
./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.o \
./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.o 

C_DEPS += \
./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.d \
./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.d \
./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/OLED_SSD1306/Src/%.o Middlewares/Third_Party/OLED_SSD1306/Src/%.su Middlewares/Third_Party/OLED_SSD1306/Src/%.cyclo: ../Middlewares/Third_Party/OLED_SSD1306/Src/%.c Middlewares/Third_Party/OLED_SSD1306/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/G/CODE/FLOOR-HEATING-IO-MODULE/IO_MODULE_proj/Middlewares/Third_Party/MODBUS-LIB/Inc" -I"C:/Users/G/CODE/FLOOR-HEATING-IO-MODULE/IO_MODULE_proj/Middlewares/Third_Party/OLED_SSD1306/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-OLED_SSD1306-2f-Src

clean-Middlewares-2f-Third_Party-2f-OLED_SSD1306-2f-Src:
	-$(RM) ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.cyclo ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.d ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.o ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306.su ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.cyclo ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.d ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.o ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_fonts.su ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.cyclo ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.d ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.o ./Middlewares/Third_Party/OLED_SSD1306/Src/ssd1306_tests.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-OLED_SSD1306-2f-Src

